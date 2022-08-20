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

#ifndef COMPONENT_NAME_HPP_
#define COMPONENT_NAME_HPP_

#include "Component.hpp"
#include <string>

struct Name : Component<Name>
{
public:
    std::string name;

    Name(std::string _name = std::string()) :
        name(_name) {}

    Name FromLua(sol::object ref)
    {
        if (ref.get_type() == sol::type::string)
            this->name = ref.as<std::string>();
        else
            throw std::string("Name component not formatted properly");

        return *this;
    }

    void serialize(cereal::JSONOutputArchive& ar) final {
        ar(CEREAL_NVP(name));
    }

    void serialize(cereal::JSONInputArchive& ar) final {
        ar(CEREAL_NVP(name));
    }

    virtual std::string serializeName(void) const final {
        return "Name";
    }
};

#endif // COMPONENT_NAME_HPP_

