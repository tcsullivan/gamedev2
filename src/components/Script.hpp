/**
 * @file Script.hpp
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

#ifndef COMPONENT_SCRIPT_HPP_
#define COMPONENT_SCRIPT_HPP_

#include "Component.hpp"

struct Scripted : Component<Scripted>, entityx::Component<Scripted>
{
public:
    sol::table caller;

    Scripted(void) {}
    Scripted(sol::table call) :
        caller(call) {}

    ~Scripted() {}

    void cleanup(void)
    {
        caller = sol::nil;
    }

    Scripted FromLua([[maybe_unused]] sol::object ref)
    {
        return *this;
    }

    void exec(void) {
        if (caller["Idle"] == sol::type::function)
            caller["Idle"](caller); // Call idle function and pass itself
                                    //  in or to fulfill the 'self' param
    }

    void update(void)
    {
        if (caller["Update"] == sol::type::function)
            caller["Update"](caller);
    }
};

#endif // COMPONENT_SCRIPT_HPP_

