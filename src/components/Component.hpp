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

#ifndef COMPONENT_HPP_
#define COMPONENT_HPP_

#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>

#include <entityx/entityx.h>
#include <sol/sol.hpp>

template<typename T>
class Component : public entityx::Component<T>
{
public:
    virtual T FromLua(sol::object) = 0;

    virtual void serialize(cereal::JSONOutputArchive& ar) = 0;
    virtual void serialize(cereal::JSONInputArchive& ar) = 0;

    void internal_serialize(bool save, void *ar) final {
        if (save)
            serialize(*reinterpret_cast<cereal::JSONOutputArchive*>(ar));
        else
            serialize(*reinterpret_cast<cereal::JSONInputArchive*>(ar));
    }
};

#endif // COMPONENT_HPP_

