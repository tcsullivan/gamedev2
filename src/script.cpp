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

// TODO move all of these below once the test printouts are gone
#include <components/Position.hpp>
#include <components/Name.hpp>
#include <components/Render.hpp>
#include <components/IdleFunc.hpp>

void ScriptSystem::doFile(void)
{
    auto result = lua.script_file("Scripts/init.lua");
    if (!result.valid()) {
        std::cout << "Lua error: " << std::endl;
    }

    manager->each<Position>(
            [&](entityx::Entity, Position& p)
            {std::cout << p.x << "," << p.y << std::endl;});

    manager->each<Name>(
            [&](entityx::Entity, Name& n)
            {std::cout << n.name << std::endl;});

    manager->each<Render>(
            [&](entityx::Entity, Render& r)
            {std::cout << r.texture << ": " << r.visible << std::endl;});
}

/********************
*  SCRIPT PARSING  *
********************/
// TODO here

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
    sol::table toRet = lua.create_table_with();
    
    if (param.get_type() == sol::type::table) {
        sol::table tab = param;

        entityx::Entity e = manager->create();

        if (tab["Position"] != nullptr) {
            toRet["Position"] = 
                e.assign<Position>(Position().FromLua(tab["Position"])).get();
        }

        if (tab["init"] != nullptr) {
            toRet["init"] = tab["init"];
        }

        if (tab["Name"] != nullptr) {
            toRet["Name"] =
                e.assign<Name>(Name().FromLua(tab["Name"])).get();
        }

        if (tab["Render"] != nullptr) {
            toRet["Render"] =
                e.assign<Render>(Render().FromLua(tab["Render"])).get();
        }

    } else {
        std::cerr << "Parameter to spawn() must be a table!" << std::endl;
    }

    return toRet;
}
