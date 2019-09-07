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

void ScriptSystem::configure(entityx::EntityManager& entities,
                             entityx::EventManager& events)
{
    this->manager = &entities;
    this->events = &events;

    events.subscribe<EntitySpawnEvent>(*this);

    // Init after systems.configure() in engine.cpp
    //init();
}

#include <components/Script.hpp>
void ScriptSystem::update([[maybe_unused]] entityx::EntityManager& entities,
                          [[maybe_unused]] entityx::EventManager& events,
                          [[maybe_unused]] entityx::TimeDelta dt)
{
    entities.each<Scripted>([](entityx::Entity, Scripted &s){
        s.updatePhysics();
    });
}


void ScriptSystem::receive([[maybe_unused]] const EntitySpawnEvent &toSpawn)
{
}

/*********************
*  CLASS FUNCTIONS  *
*********************/

int ScriptSystem::init(void)
{
    lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string);

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
#include <components/EventListener.hpp>
#include <components/Position.hpp>
#include <components/Player.hpp>
#include <components/Name.hpp>
#include <components/Render.hpp>
#include <components/Script.hpp>
#include <components/Velocity.hpp>
#include <components/Light.hpp>
#include <components/Physics.hpp>

void ScriptSystem::scriptExport(void)
{
    std::function<sol::table(sol::table)> func = 
        [this](sol::table t){ return spawn(t);};

    lua.new_usertype<Position>("Position",
            sol::constructors<Position(double x, double y), Position()>(),
            "x", &Position::x,
            "y", &Position::y);

    lua.new_usertype<Name>("Name",
            sol::constructors<Name(std::string), Name()>(),
            "value", &Name::name);

    lua.new_usertype<Render>("Render",
            sol::constructors<Render(std::string), Render()>(),
            "visible", &Render::visible,
            "texture", &Render::texture,
            "flipx", &Render::flipX);

    lua.new_usertype<Velocity>("Velocity",
            sol::constructors<Velocity(double, double), Velocity()>(),
            "x", &Velocity::x,
            "y", &Velocity::y);

    lua.new_usertype<Player>("Player",
            sol::constructors<Player(void), Player()>());

    lua.new_usertype<Light>("Light",
            sol::constructors<Light(float, float, float, float)>(),
            "r", &Light::r,
            "g", &Light::g,
            "b", &Light::b,
            "strength", &Light::strength);

    lua.new_usertype<Physics>("Physics",
            sol::constructors<Physics(void), Physics()>(),
            "standing", &Physics::standing);

    auto gamespace = lua["game"].get_or_create<sol::table>();
    gamespace.set_function("spawn", func);
}

sol::table ScriptSystem::spawn(sol::object param)
{
    sol::table* toRet; // "Entitiy" table to be returned
    if (param.get_type() == sol::type::table) {
        sol::table tab = param; // Cast the generic parameter to a table

        entityx::Entity e = manager->create(); // Create a new entity
        auto d = e.assign<Scripted>().get(); // Since this entity was created
                                             //  via Lua, assign the Scripted
                                             //  component.
        d->caller = lua.create_table(); // Create our Entity "table"
        toRet = &(d->caller); // set our return table to the Entity "table"

        *toRet = tab; /* Copy table to our new entity to preserve functions
                      *   this reduces the amount of work done in this function
                      *   as well. We are allowed to do this because lua allows
                      *   for the reallocation of types.
                      */

        /*
         * Create the component dependancies (i.e.: position) first
         */
        if (tab["Position"] != nullptr) {
            (*toRet)["Position"] = 
                e.assign<Position>(Position().FromLua(tab["Position"])).get();
        }

        if (tab["Name"] != nullptr) {
            (*toRet)["Name"] =
                e.assign<Name>(Name().FromLua(tab["Name"])).get();
        }

        if (tab["Render"] != nullptr) {
            if (!e.has_component<Position>()) // Position must exist for render
                (*toRet)["Position"] = e.assign<Position>().get();
            (*toRet)["Render"] =
                e.assign<Render>(Render().FromLua(tab["Render"])).get();
        }

        if (tab["Velocity"] != nullptr) {
            if (!e.has_component<Position>()) // Position must exist for vel.
                (*toRet)["Position"] = e.assign<Position>().get();
            (*toRet)["Velocity"] =
                e.assign<Velocity>(Velocity().FromLua(tab["Velocity"])).get();
        }

        if (tab["Player"] != nullptr) {
            (*toRet)["Player"] = e.assign<Player>().get();
        }

        if (tab["Physics"] != nullptr) {
            if (!e.has_component<Position>()) // Position must exist for phys.
                (*toRet)["Position"] = e.assign<Position>().get();
            if (!e.has_component<Velocity>()) // Velocity must exist for phys.
                (*toRet)["Velocity"] = e.assign<Velocity>().get();

            (*toRet)["Physics"] = e.assign<Physics>().get();
        }

        if (tab["Light"] != nullptr) {
            if (!e.has_component<Position>()) // Position must exist for vel.
                (*toRet)["Position"] = e.assign<Position>().get();
            (*toRet)["Light"] = 
                e.assign<Light>(Light().FromLua(tab["Light"])).get();
        }

        // Probably should be last
        if (tab["EventListeners"] != nullptr) {
            sol::table listeners = tab["EventListeners"];
            e.assign<EventListener>(listeners);
        }

    } else {
        // TODO better logging
        std::cerr << "Parameter to spawn() must be a table!" << std::endl;
    }

    // If the Entity has an Init function defined, call it
    if ((*toRet)["Init"].get_type() == sol::type::function)
        (*toRet)["Init"](*toRet);

    return *toRet;
}

