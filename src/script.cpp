/**
 * @file script.cpp
 * Manages all Lua scripts.
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

#include <script.hpp>

/*********************
*  SYSTEM SPECIFIC  *
*********************/

void ScriptSystem::configure([[maybe_unused]]entityx::EntityManager& entities,
                             [[maybe_unused]]entityx::EventManager& events)
{
    this->manager = &entities;
    this->events = &events;

    events.subscribe<EntitySpawnEvent>(*this);

    init();
}

void ScriptSystem::update([[maybe_unused]] entityx::EntityManager& entites,
                          [[maybe_unused]] entityx::EventManager& events,
                          [[maybe_unused]] entityx::TimeDelta dt)
{

}


void ScriptSystem::receive(const EntitySpawnEvent &toSpawn)
{
    (void)toSpawn;
}

/*********************
*  CLASS FUNCTIONS  *
*********************/

int ScriptSystem::init(void)
{
    lua.open_libraries(sol::lib::base);

    scriptExport();
    doFile();

    return 0;
}

void ScriptSystem::doFile(void)
{

    auto result = lua.script_file("Scripts/init.lua");

    if (!result.valid()) {
        std::cout << "Lua error: " << std::endl;
    }
}

/********************
*  SCRIPT PARSING  *
********************/
#include <components/Position.hpp>
#include <components/Name.hpp>
#include <components/Render.hpp>
#include <components/Script.hpp>

void ScriptSystem::scriptExport()
{

    std::function<sol::table(sol::table)> func = 
        [this](sol::table t){ return spawn(t);};

    lua.new_usertype<Position>("Position",
            sol::constructors<Position(float x, float y), Position()>(),
            "x", &Position::x,
            "y", &Position::y);

    lua.new_usertype<Name>("Name",
            sol::constructors<Name(std::string), Name()>(),
            "value", &Name::name);

    lua.new_usertype<Render>("Render",
            sol::constructors<Render(std::string), Render()>(),
            "visible", &Render::visible,
            "texture", &Render::texture);

    auto gamespace = lua["game"].get_or_create<sol::table>();
    gamespace.set_function("spawn", func);
}

sol::table ScriptSystem::spawn(sol::object param)
{
    sol::table* toRet;
    if (param.get_type() == sol::type::table) {
        sol::table tab = param;

        entityx::Entity e = manager->create();
        auto d = e.assign<Scripted>().get();
        d->caller = lua.create_table();
        toRet = &(d->caller);

        *toRet = tab; // Copy table to our new entity to preserve functions
                      //  this reduces the amount of work done in this function
                      //  as well

        if (tab["Position"] != nullptr) {
            (*toRet)["Position"] = 
                e.assign<Position>(Position().FromLua(tab["Position"])).get();
        }

        if (tab["Name"] != nullptr) {
            (*toRet)["Name"] =
                e.assign<Name>(Name().FromLua(tab["Name"])).get();
        }

        if (tab["Render"] != nullptr) {
            (*toRet)["Render"] =
                e.assign<Render>(Render().FromLua(tab["Render"])).get();
        }

    } else {
        std::cerr << "Parameter to spawn() must be a table!" << std::endl;
    }

    return *toRet;
}
