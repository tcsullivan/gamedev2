/**
 * @file texture.cpp
 * Handles all texture loading 
 *
 * Copyright (C) 2019  Belle-Isle, Andrew <drumsetmonkey@gmail.com>
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


#include "texture.hpp"

#include <soil/SOIL.h>

#include <unordered_map>
#include <iostream>

#include <script/vectors.hpp>

struct TextureData
{
    GLuint tex = 0;
    int width = 0;
    int height= 0;
};

// Stores a list of all textures we've already loaded. This makes sure we don't
// waste our precious CPU cycles reloading a texture.
std::unordered_map<std::string, TextureData> textureCache;

TextureData loadTexture(std::string filename)
{
    TextureData tex;

    // Search to see if this texture has already been loading
    auto cacheSearch = textureCache.find(filename);

    if (cacheSearch == textureCache.end()) {
    // If this texture hasn't been loading

        unsigned char* image = SOIL_load_image(filename.c_str(),
                                               &(tex.width), &(tex.height), 0,
                                               SOIL_LOAD_RGBA);

        glGenTextures(1, &(tex.tex));
        glBindTexture(GL_TEXTURE_2D, tex.tex);
        glPixelStoref(GL_UNPACK_ALIGNMENT, 1);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width, tex.height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, image);

        SOIL_free_image_data(image);

        // Add new texture to the texture cache
        textureCache.emplace(filename, tex);

    } else {
    // If this texture has been loaded, just return the loaded texture
        tex = cacheSearch->second;
    }

    return tex;
}

void Texture::loadFromString(std::string filename)
{
    TextureData d = loadTexture(filename);

    tex = d.tex;
    width = d.width;
    height = d.height;
}

Texture::Texture(std::string filename)
{
    loadFromString(filename);
}

Texture::Texture(sol::object param)
{
    if (param.get_type() == sol::type::string) {
        loadFromString(param.as<std::string>());
    } else if (param.get_type() == sol::type::table) {
        sol::table tab = param;

        // If there is a filename given, load that file to get image data
        if (tab["file"] == sol::type::string)
            loadFromString(tab.get<std::string>("file"));
        else
            return; // If we don't have image data just return a null image

        if (tab["offset"].get_type() == sol::type::table)
            offset = Script::to<glm::vec2>(tab["offset"]);

        if (tab["size"].get_type() == sol::type::table)
            size = Script::to<glm::vec2>(tab["size"]);
    }
}
