/**
 * @file Text.hpp
 * Allows text to be shown with an entity.
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

#ifndef COMPONENT_TEXT_HPP_
#define COMPONENT_TEXT_HPP_

#include "Component.hpp"

struct Text : Component<Text>
{
public:
    std::string font;
    std::string text;
    double offsetX, offsetY;

    Text(const std::string& _font, const std::string& _text) :
        font(_font), text(_text), offsetX(0), offsetY(0) {}
    Text(const std::string& _font, const std::string& _text,
         double _x, double _y) :
        font(_font), text(_text), x(_x), y(_y) {}

    Text FromLua(sol::object ref)
    {
        if (ref.get_type() == sol::type::table) {
            sol::table tab = ref;
            if (tab["font"] != nullptr)
                this->font = tab["font"];
            if (tab["text"] != nullptr)
                this->text = tab["text"];
            if (tab["x"] != nullptr)
                this->x = tab["x"];
            if (tab["y"] != nullptr)
                this->y = tab["y"];
        } else {
            throw std::string("Text table not formatted properly");
        }
        return *this;
    }

    void serialize(cereal::JSONOutputArchive& ar) final {
        ar(CEREAL_NVP(font), CEREAL_NVP(text), CEREAL_NVP(x), CEREAL_NVP(y));
    }

    void serialize(cereal::JSONInputArchive& ar) final {
        ar(CEREAL_NVP(font), CEREAL_NVP(text), CEREAL_NVP(x), CEREAL_NVP(y));
    }

    std::string serializeName(void) const final {
        return "Text";
    }
};

#endif // COMPONENT_TEXT_HPP_

