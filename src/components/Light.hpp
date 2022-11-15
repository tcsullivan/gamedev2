/**
 * @file Light.hpp
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

#ifndef COMPONENT_LIGHT_HPP_
#define COMPONENT_LIGHT_HPP_

#include "Component.hpp"

struct Light : Component<Light>
{
public:
    float r, g, b;
    float strength;

    Light() {}
    Light(float _r, float _g, float _z, float _s) :
        r(_r), g(_g), b(_z), strength(_s) {}

    Light FromLua(sol::object ref)
    {
        if (ref.get_type() == sol::type::table) {
            sol::table tab = ref;
            if (tab["r"] == sol::type::number)
                this->r = tab["r"];
            if (tab["g"] == sol::type::number)
                this->g = tab["g"];
            if (tab["b"] == sol::type::number)
                this->b = tab["b"];
            if (tab["strength"] == sol::type::number)
                this->strength = tab["strength"];
        } else {
            throw std::string("Light component must be a table");
        }
        return *this;
    }

    void serialize(cereal::JSONOutputArchive& ar) final {
        ar(CEREAL_NVP(r), CEREAL_NVP(g), CEREAL_NVP(b), CEREAL_NVP(strength));
    }

    void serialize(cereal::JSONInputArchive& ar) final {
        ar(CEREAL_NVP(r), CEREAL_NVP(g), CEREAL_NVP(b), CEREAL_NVP(strength));
    }

    virtual std::string serializeName(void) const final {
        return "Light";
    }
};

#endif//COMPONENT_LIGHT_HPP_
