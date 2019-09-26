/**
 * @file text.hpp
 * Manages entity text.
 *
 * Copyright (C) 2019 Clyne Sullivan
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// hello

#ifndef SYSTEM_TEXT_HPP_
#define SYSTEM_TEXT_HPP_

#include <entityx/entityx.h>
#include <ft2build.h>
#include <freetype/freetype.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

#include <map>
#include <string>
#include <tuple>
#include <vector>

struct TextMeshData
{
    float posX, posY, posZ;
    float texX, texY;
    float transparency;
} __attribute__ ((packed));

struct FT_Info {
    std::pair<float, float> offset;
    std::pair<float, float> dim;
    std::pair<float, float> bitmap;
    std::pair<float, float> advance;
};

struct Text {
    std::string text;
    float x;
    float y;
    float z;

    Text(std::string _text, float _x, float _y, float _z = 0.0f) :
        text(_text), x(_x), y(_y), z(_z) {}
};

// Stores texture and placement data for a font at a size.
struct Font {
    int fontSize;

    GLuint tex;
    GLuint vbo;

    float width;
    float height;

    std::array<FT_Info, 96> data;
    // Stores currently shown text at given index into VBO?
    std::vector<Text> text;
    std::basic_string<TextMeshData> buffer;
};

class TextSystem : public entityx::System<TextSystem>
{
public:
    /**
     * Prepares the system for running.
     */
    void configure(entityx::EntityManager& entities,
                   entityx::EventManager& events) final;
    
    /**
     * Draws the text for all entities.
     */
    void update(entityx::EntityManager& entites,
                entityx::EventManager& events,
                entityx::TimeDelta dt) final;

    void put(const std::string& font, float x, float y, const std::string& text);

    void loadFont(const std::string& name, const std::string& file, int size);

private:
    FT_Library freetype;
    std::map<std::string, FT_Face> fonts;
    std::map<std::string, Font> fontData;
    bool shouldUpdateVBOs;

    void updateVBOs(void);
};

#endif // SYSTEM_TEXT_HPP_

