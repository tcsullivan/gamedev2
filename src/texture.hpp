/**
 * @file texture.hpp
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

#ifndef TEXTURE_HPP_
#define TEXTURE_HPP_

#include <soil/SOIL.h>

#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

#include <string>

class Texture
{
private:
public:
    GLuint tex;
    int width;
    int height;
    Texture() {};
    Texture(std::string);
};

#endif//TEXTURE_HPP_
