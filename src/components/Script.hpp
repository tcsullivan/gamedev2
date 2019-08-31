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

#ifndef SCRIPT_COMPONENT_HPP_
#define SCRIPT_COMPONENT_HPP_

#include <components/Component.hpp>

struct Scripted : Component<Scripted>, entityx::Component<Scripted>
{
    public:
        sol::table caller;
        Scripted() {}
        Scripted(sol::table call): caller(call) {}


        ~Scripted()
        {}

        void cleanup()
        {
            caller = sol::nil;
        }

        Scripted FromLua(sol::object)
        {
            //if (ref.get_type() == sol::type::function) {
            //    this->luafunc = ref;
            //}
            //init = true;
            return *this;
        }

        void exec() {
            if (caller["Idle"] == sol::type::function)
                caller["Idle"](caller); // Call idle function and pass itself
                                        //  in or to fulfill the 'self' param
        }
};

#endif//SCRIPT_COMPONENT_HPP_
