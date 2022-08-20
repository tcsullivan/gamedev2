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
#include <cereal/types/vector.hpp>
#include <cereal/types/tuple.hpp>

struct Scripted : Component<Scripted>
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

    void exec(void)
    {
        if (caller["Idle"] == sol::type::function)
            caller["Idle"](caller); // Call idle function and pass itself
                                    //  in or to fulfill the 'self' param
    }

    void updatePhysics(void)
    {
        if (caller["PhysicsIdle"] == sol::type::function)
            caller["PhysicsIdle"](caller);
    }

    void updateRender(void)
    {
        if (caller["RenderIdle"] == sol::type::function)
            caller["RenderIdle"](caller);
    }

    void serialize(cereal::JSONOutputArchive& ar) final {
        std::vector<std::tuple<std::string, std::string>> table_components;
        caller.for_each([&table_components](sol::object key, sol::object value){
            if (value.get_type() == sol::type::string)
                table_components.push_back(std::make_tuple(
                    key.as<std::string>(),
                    std::string("return \"" + value.as<std::string>() + "\"")
                ));
            else if (value.get_type() == sol::type::number)
                table_components.push_back(std::make_tuple(
                    key.as<std::string>(),
                    std::string("return " + value.as<std::string>())
                ));
            else if (value.get_type() == sol::type::boolean) {
                table_components.push_back(std::make_tuple(
                    key.as<std::string>(),
                    std::string("return " + 
                        std::string(value.as<bool>() ? "true" : "false")
                    )
                ));
            }
            //else if (value.get_type() == sol::type::function) {
            //    sol::state lua;
            //    lua.open_libraries(sol::lib::base, sol::lib::string);

            //    sol::function dump = lua.script("return string.dump");

            //    sol::function f = value;
            //    std::string gg = dump(f);
            //    table_components.push_back(std::make_tuple(
            //        key.as<std::string>(),
            //        std::string("return (loadstring or load)(" + 
            //                    gg + ")")
            //    ));
            //}
        });
        
        ar(CEREAL_NVP(table_components));
    }

    void serialize(cereal::JSONInputArchive& ar) final {
        sol::state lua;
        lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string);

        std::vector<std::tuple<std::string, std::string>> table_components;
        ar(CEREAL_NVP(table_components));

        for (auto &s : table_components) {
            std::string key = std::get<0>(s);
            std::string value = std::get<1>(s);
            sol::object ret = lua.script(value);
            caller[key.c_str()] = ret;
        }

    }

    virtual std::string serializeName(void) const final {
        return "Scripted";
    }
};

#endif // COMPONENT_SCRIPT_HPP_

