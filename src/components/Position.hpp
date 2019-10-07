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

struct Position : Component<Position>
{
public:
    double x, y;

    Position(double _x = 0, double _y = 0) :
        x(_x), y(_y) {}

    Position FromLua(sol::object ref)
    {
        glm::vec2 vec = Script::to<glm::vec2>(ref);
        this->x = vec.x;
        this->y = vec.y;

        return *this;
    }

    void serialize(cereal::JSONOutputArchive& ar) final {
        ar(CEREAL_NVP(x), CEREAL_NVP(y));
    }

    void serialize(cereal::JSONInputArchive& ar) final {
        ar(CEREAL_NVP(x), CEREAL_NVP(y));
    }

    std::string serializeName(void) const final {
        return "Position";
    }
};

#endif // COMPONENT_POSITION_HPP_

