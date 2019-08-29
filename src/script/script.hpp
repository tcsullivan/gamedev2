/*
 * Copyright (C) 2019  Belle-Isle, Andrew <drumsetmonkey@gmail.com>
 * Author: Belle-Isle, Andrew <drumsetmonkey@gmail.com>
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

#ifndef SCRIPT_NAMESPACE_HPP_
#define SCRIPT_NAMESPACE_HPP_

#include <sol/sol.hpp>
#include <entityx/entityx.h>

#include <components/Position.hpp>

#include <iostream>

entityx::EventManager events;
entityx::EntityManager manager(events);
entityx::Entity e;

namespace Script
{
    sol::state lua;

    sol::table newPosition([[maybe_unused]]sol::table tab)
    {
        sol::table toRet = lua.create_table_with();

        e.assign<Position>(4.5, 2.3).get();

        toRet["Position"] = e.component<Position>().get();

        return toRet;
    }

    void CreateNewState()
    {
        e = manager.create();

        lua.open_libraries(sol::lib::base);

        //lua["q"] = Position(4.5, 3.4);

        lua.new_usertype<Position>("Position",
                sol::constructors<Position(float x, float y), Position()>(),
                "x", &Position::x,
                "y", &Position::y);

        auto gamespace = lua["game"].get_or_create<sol::table>();
        gamespace.set_function("testfunc", newPosition);

        lua.script_file("Scripts/init.lua");
        auto p = e.component<Position>().get();
        std::cout << p->x << "," << p->y << std::endl;
    }
}

#endif//SCRIPT_NAMESPACE_HPP_
