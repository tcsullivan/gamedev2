#include "text.hpp"

#include "events/render.hpp"

#include <iostream>

TextSystem::~TextSystem(void)
{
    for (auto [name, face] : fonts)
        FT_Done_Face(face);

    FT_Done_FreeType(freetype);
}

void TextSystem::configure([[maybe_unused]] entityx::EntityManager& entities,
                           entityx::EventManager& events)
{
    shouldUpdateVBOs = false;

    events.subscribe<ShowTextEvent>(*this);

    if (FT_Init_FreeType(&freetype) != 0) {
        // TODO handle error
    }
}
    
/**
 * Draws the text for all entities.
 */
void TextSystem::update([[maybe_unused]] entityx::EntityManager& entites,
                        entityx::EventManager& events,
                        [[maybe_unused]] entityx::TimeDelta dt)
{
    if (shouldUpdateVBOs) {
        shouldUpdateVBOs = false;
        updateVBOs();

        for (auto& [name, font] : fontData) {
            if (font.text.size() != 0) {
                events.emit<NewRenderEvent>(font.vbo, font.tex, 0,
                                            font.buffer.size());
            }
        }
    }
}

void TextSystem::receive(const ShowTextEvent& ste)
{
    put(ste.font, ste.x, ste.y, ste.text);
}

void TextSystem::loadFont(const std::string& name,
                          const std::string& file,
                          int size)
{
    // Find or load font at given size
    //

    if (fonts.find(file) == fonts.end()) {
        FT_Face face;
        if (FT_New_Face(freetype, file.c_str(), 0, &face)) {
            // TODO handle this error
        }
        fonts.emplace(file, face);
    }

    auto& face = fonts[file];
    FT_Set_Pixel_Sizes(face, 0, size);
    fontData.try_emplace(name);
    fontData[name].fontSize = size;

    // Calculate dimensions of final texture
    //

    float width = 0, height = 0;
    for (int c = 32; c < 128; c++) {
        FT_Load_Char(face, c, FT_LOAD_RENDER);
        width += face->glyph->bitmap.width + 1;
        height = std::max(height, static_cast<float>(face->glyph->bitmap.rows));
    }

    // Generate texture to hold entire font
    //

    auto& font = fontData[name];
    glGenBuffers(1, &font.vbo);

    glGenTextures(1, &font.tex);
    glBindTexture(GL_TEXTURE_2D, font.tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_INTENSITY8, width, height,
                 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, 0);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //    // convert red-on-black to RGBA
    //    auto& g = face->glyph;
    //    std::vector<uint32_t> buf (g->bitmap.width * g->bitmap.rows, 0xFFFFFF);
    //    for (auto j = buf.size(); j--;)
    //        buf[j] |= g->bitmap.buffer[j] << 24;

    // Load each character and add it to the texture
    //

    font.width = width;
    font.height = height;

    float offsetX = 0, offsetY = 0;
    for (int c = 32; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "Unrecognized character: " << c << std::endl;
            continue;
        }

        auto* g = face->glyph;
        glTexSubImage2D(GL_TEXTURE_2D, 0, offsetX, offsetY,
                        g->bitmap.width, g->bitmap.rows,
                        GL_LUMINANCE, GL_UNSIGNED_BYTE,
                        g->bitmap.buffer);

        auto& d = font.data[c-32];
        d.dim = { g->bitmap.width, g->bitmap.rows };
        d.bitmap = { g->bitmap_left, g->bitmap_top };
        d.advance = { g->advance.x >> 6, g->advance.y >> 6 };

        d.offset = { offsetX / width, offsetY / height };
        offsetX += g->bitmap.width;// + 1.0f;
        // Keep offsetY at zero?
    }

    std::cout << "Loaded font: " << file << " (size: " << size << ", tex: "
              << font.tex << ")" << std::endl;
}

void TextSystem::put(const std::string& font,
                     float x,
                     float y,
                     const std::string& text)
{
    if (fontData.find(font) == fontData.end())
        return;
    auto& vector = fontData[font].text;

    y = -(y + fontData[font].fontSize);

    const auto it = std::find_if(vector.begin(), vector.end(),
        [&x, &y](const Text& t) {
            return t.x == static_cast<int>(x) && t.y == static_cast<int>(y);
        });
    if (it != vector.end()) {
        *it = Text(text, x, y);
    } else {
        // Invert y axis so positive grows south.
        fontData[font].text.emplace_back(text, x, y);
    }

    shouldUpdateVBOs = true;
}

void TextSystem::updateVBOs(void)
{
    for (auto& [name, d] : fontData) {
        d.buffer.clear();
        for (auto& text : d.text) {
            float tx = text.x;
            float ty = text.y;
            for (char c : text.text) {
                if (c < 32)
                    continue;

                c -= 32;

                float x = tx + d.data[c].bitmap.first;
                float y = ty - (d.data[c].dim.second - d.data[c].bitmap.second);
                float z = text.z;

                float w = d.data[c].dim.first;
                float h = d.data[c].dim.second;

                tx += d.data[c].advance.first;
                ty += d.data[c].advance.second;

                if (w == 0.0f || h == 0.0f)
                    continue;

                d.buffer += {
                    x,
                    y,
                    z,
                    d.data[c].offset.first,
                    d.data[c].offset.second+d.data[c].dim.second/d.height,
                    1.0f
                };
                d.buffer += {
                    x+w,
                    y,
                    z,
                    d.data[c].offset.first+d.data[c].dim.first/d.width, 
                    d.data[c].offset.second+d.data[c].dim.second/d.height,
                    1.0f
                };
                d.buffer += {
                    x,
                    y+h,
                    z,
                    d.data[c].offset.first, 
                    d.data[c].offset.second,
                    1.0f
                };

                d.buffer += {
                    x+w,
                    y,
                    z,
                    d.data[c].offset.first+d.data[c].dim.first/d.width,
                    d.data[c].offset.second+d.data[c].dim.second/d.height,
                    1.0f
                };
                d.buffer += {
                    x+w,
                    y+h,
                    z,
                    d.data[c].offset.first+d.data[c].dim.first/d.width,
                    d.data[c].offset.second,
                    1.0f
                };
                d.buffer += {
                    x,
                    y+h,
                    z,
                    d.data[c].offset.first,
                    d.data[c].offset.second,
                    1.0f
                };
            }
        }

        glBindBuffer(GL_ARRAY_BUFFER, d.vbo);
        glBufferData(GL_ARRAY_BUFFER,
                     d.buffer.size() * sizeof(TextMeshData), d.buffer.data(),
                     GL_DYNAMIC_DRAW);
    }
}

