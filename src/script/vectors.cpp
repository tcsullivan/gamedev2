/*
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

#include "vectors.hpp"

#include <glm/glm.hpp>

#include <iostream>

namespace Script
{
    template<class T>
    T to(sol::object obj)
    {
        (void)obj;
        T fake;
        return fake;
    }

    template<>
    glm::vec2 to<glm::vec2>(sol::object obj)
    {
        glm::vec2 toReturn;

        if (obj.get_type() == sol::type::table) {
            sol::table table = obj;
            // X
            if (table["x"] == sol::type::number) {
                toReturn.x = table["x"];
            } else if (table[1] == sol::type::number) {
                toReturn.x = table[1];
            }
            // Y
            if (table["y"] == sol::type::number) {
                toReturn.y = table["y"];
            } else if (table[2] == sol::type::number) {
                toReturn.y = table[2];
            }
        } else {
            std::cerr << "Vectors must be in table form" << std::endl;
        }

        return toReturn;
    }

    template<>
    glm::vec3 to<glm::vec3>(sol::object obj)
    {
        glm::vec3 toReturn;

        if (obj.get_type() == sol::type::table) {
            sol::table table = obj;
            glm::vec2 base = to<glm::vec2>(table);
            toReturn.x = base.x;
            toReturn.y = base.y;
            
            // Z
            if (table["z"] == sol::type::number) {
                toReturn.z = table["z"];
            } else if (table[3] == sol::type::number) {
                toReturn.z = table[3];
            }
        } else {
            std::cerr << "Vectors must be in table form" << std::endl;
        }

        return toReturn;
    }

    template<>
    glm::vec4 to<glm::vec4>(sol::object obj)
    {
        glm::vec4 toReturn;

        if (obj.get_type() == sol::type::table) {
            sol::table table = obj;
            glm::vec3 base = to<glm::vec3>(table);
            toReturn.x = base.x;
            toReturn.y = base.y;
            toReturn.z = base.z;
            
            // W
            if (table["w"] == sol::type::number) {
                toReturn.w = table["w"];
            } else if (table[4] == sol::type::number) {
                toReturn.w = table[4];
            }
        } else {
            std::cerr << "Vectors must be in table form" << std::endl;
        }

        return toReturn;
    }
}
