#include "text.hpp"

#include <iostream>

void TextSystem::configure([[maybe_unused]] entityx::EntityManager& entities,
                           [[maybe_unused]] entityx::EventManager& events)
{
    if (FT_Init_FreeType(&freetype) != 0) {
        // TODO handle error
    }
}
    
/**
 * Draws the text for all entities.
 */
void TextSystem::update([[maybe_unused]] entityx::EntityManager& entites,
                        [[maybe_unused]] entityx::EventManager& events,
                        [[maybe_unused]] entityx::TimeDelta dt)
{
    // TODO render each Text component's text
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
    glGenTextures(1, &font.tex);
    glGenBuffers(1, &font.vbo);
    glBindTexture(GL_TEXTURE_2D, font.tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height,
                 0, GL_RED, GL_UNSIGNED_BYTE, 0);
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

    float offsetX = 0, offsetY = 0;
    for (int c = 32; c < 128; c++) {
        FT_Load_Char(face, c, FT_LOAD_RENDER);

        auto* g = face->glyph;
        glTexSubImage2D(GL_TEXTURE_2D, 0, offsetX, offsetY,
                        g->bitmap.width, g->bitmap.rows,
                        GL_RED, GL_UNSIGNED_BYTE,
                        g->bitmap.buffer);

        auto& d = font.data[c - 32];
        d.dim = { g->bitmap.width, g->bitmap.rows };
        d.bitmap = { g->bitmap_left, g->bitmap_top };
        d.advance = { g->advance.x >> 6, g->advance.y >> 6 };

        d.offset = { offsetX / width, offsetY / height };
        offsetX += g->bitmap.width;
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

    fontData[font].text.emplace_back(text, x, y);
}

void TextSystem::updateVBOs(void)
{
    for (auto& data : fontData) {
        auto& d = data.second;
        d.buffer.clear();
        for (auto& text : d.text) {
            for (char c : text.text) {
                if (c < 32)
                    continue;

                d.buffer += {
                    text.x, text.y, text.z,
                    d.data[c].offset.first, d.data[c].offset.second,
                    1.0f
                };
            }
        }

        glBindBuffer(GL_ARRAY_BUFFER, d.vbo);
        glBufferData(GL_ARRAY_BUFFER,
                     d.text.size() * sizeof(TextMeshData), d.text.data(),
                     GL_STREAM_DRAW);
    }
}

