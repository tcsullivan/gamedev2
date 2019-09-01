/**

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

#ifndef COMPONENT_POSITION_HPP_
#define COMPONENT_POSITION_HPP_

#include "Component.hpp"

struct Position : Component<Position>, entityx::Component<Position>
{
public:
    double x, y;

    Position(double _x = 0, double _y = 0) :
        x(_x), y(_y) {}

    Position FromLua(sol::object ref)
    {
        if (ref.get_type() == sol::type::table) {
            sol::table tab = ref;
            if (tab["x"] != nullptr)
                this->x = tab["x"];
            if (tab["y"] != nullptr)
                this->y = tab["y"];
        } else {
            throw std::string("Position table not formatted properly");
        }
        return *this;
    }
};

#endif // COMPONENT_POSITION_HPP_

