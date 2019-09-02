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

#ifndef COMPONENT_RENDER_HPP_
#define COMPONENT_RENDER_HPP_

#include "Component.hpp"
#include "texture.hpp"

struct Render : Component<Render>
{
public:
    Texture texture;
    Texture normal;
    bool visible;
    bool flipX = false;

    Render(std::string _file) :
        texture(_file), visible(true) {}
    Render(void) :
        texture(), visible(false) {}

    Render FromLua(sol::object ref)
    {
        if (ref.get_type() == sol::type::table) {
            sol::table tab = ref;
            if (tab["visible"].get_type() == sol::type::boolean)
                this->visible = tab["visible"];
            if (tab["texture"].get_type() == sol::type::string)
                this->texture = Texture(tab.get<std::string>("texture"));
            if (tab["normal"].get_type() == sol::type::string)
                this->normal = Texture(tab.get<std::string>("normal"));
            if (tab["flipx"].get_type() == sol::type::boolean)
                this->flipX = tab["flipx"];
        } else {
            throw std::string(
                "Render component table formatted incorrectly"
            );
        }
        return *this;
    }

    void serialize(cereal::JSONOutputArchive& ar) final {
        ar(CEREAL_NVP(visible), CEREAL_NVP(flipX));
    }

    void serialize(cereal::JSONInputArchive& ar) final {
        ar(CEREAL_NVP(visible), CEREAL_NVP(flipX));
    }

    std::string serializeName(void) const final {
        return "Render";
    }
};

#endif // COMPONENT_RENDER_HPP_

