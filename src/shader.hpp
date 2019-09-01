/**
 * @file shader.hpp
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

#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <unordered_map>

#include <GL/glew.h>

class Shader
{
    private:
        /**
         * Reads the contents of a shader file and returns a c++ string
         * object representing the contents
         * @param The file path
         * @return The shader contents
         */
        std::string readShader(std::string); 

        /**
         * Creates a shader from a filename and a shader type
         * @param The file path containing the shader data
         * @param What type of shader to create
         * @return Memory address of shader location
         */
        GLuint createShader(std::string, GLenum);

        GLuint program; /**< GPU Memory address of shader program */

        /**
         * Name of shader attributes and their corresponding memory
         * locations in which the data exists
         */
        std::unordered_map<std::string, GLuint> attributes;
        
        /**
         * Name of shader uniforms in and their corresponding memory
         * locations in which the data exists
         */
        std::unordered_map<std::string, GLuint> uniforms;
    public:
        Shader(): program(-1) {}
        /**
         * Given the file paths of two shaders, create a shader program.
         * @param v The file path of the vertex shader file.
         * @param f The file path of the fragment shader file.
         * @return The GPU Memory location of the shader program
         */
        GLuint createProgram(std::string v, std::string f); 

        /**
         * Finds and binds an attribute to the current shader if possible
         * @param The attribute to bind in the shader
         * @return The memory address of the new attribute, or -1 if the
         * attribute doesn't exist in the shader
         */
        GLint addAttribute(std::string);
        /**
         * Finds and binds a uniform to the current shader if possible
         * @param The uniform to bind in the shader
         * @return The memory address of the new uniform, or -1 if the
         * uniform doesn't exist in the shader
         */
        GLint addUniform(std::string);

        /**
         * Finds the GPU memory address of the given attribute in the shader
         * program
         * @param The attribute to find
         * @return The attribute memory location, or -1 if it doesn't exist
         */
        GLint getAttribute(std::string);
        /**
         * Finds the GPU memory address of the given uniform in the shader
         * program
         * @param The uniform to find
         * @return The uniform memory location, or -1 if it doesn't exist
         */
        GLint getUniform(std::string);

        /**
         * Gets the memory address of the program stored in this object
         * @return The GPU memory address of the shader program stored
         */
        GLuint getProgram();
};

#endif // SHADER_HPP
