#include "text.hpp"

#include <SDL2/SDL_opengl.h>

struct FT_Info {
	vec2 wh;
	vec2 bl;
	vec2 ad;
	GLuint tex;

	FT_Info(void)
		: tex(0) {}
};

//FT_Library freetype;
//std::map<std::string, FT_Face> fonts;
//std::map<std::string, std::vector<FT_Info>> fontData;

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

    if (fonts.find(name) == fonts.end()) {
        FT_Face face;
        if (FT_New_Face(freetype, file.c_str(), 0, &face)) {
            // TODO handle this error
        }
        fonts.emplace(name, face);
    }

    auto& face = fonts[name];
    FT_Set_Pixel_Sizes(face, 0, size);
    fontData.try_emplace(name, 95);

    char c = 32;
    for (auto& d : fontData[name]) {
        FT_Load_Char(face, c++, FT_LOAD_RENDER);

        glGenTextures(1, &d.tex);
        glBindTexture(GL_TEXTURE_2D, d.tex);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S , GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T , GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER , GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER , GL_LINEAR);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // convert red-on-black to RGBA
        auto& g = face->glyph;
        std::vector<uint32_t> buf (g->bitmap.width * g->bitmap.rows, 0xFFFFFF);
        for (auto j = buf.size(); j--;)
            buf[j] |= g->bitmap.buffer[j] << 24;

        d.wh.x = g->bitmap.width;
        d.wh.y = g->bitmap.rows;
        d.bl.x = g->bitmap_left;
        d.bl.y = g->bitmap_top;
        d.ad.x = g->advance.x >> 6;
        d.ad.y = g->advance.y >> 6;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, g->bitmap.width, g->bitmap.rows,
            0, GL_RGBA, GL_UNSIGNED_BYTE, buf.data());
    }
}

