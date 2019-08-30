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

#ifndef SCRIPT_HPP_
#define SCRIPT_HPP_

#include <entityx/entityx.h>
#include <lua.hpp>
#include <sol/sol.hpp>

/****************
*  COMPONENTS  *
****************/
#include <components/Position.hpp>

struct EntitySpawnEvent {
    EntitySpawnEvent (sol::object ref)
        : ref(ref) {}

    sol::object ref;
};

/**
 * @class ScriptSystem
 * Handles all the game's scripting requests
 */
class ScriptSystem : public entityx::System<ScriptSystem>
                   , public entityx::Receiver<ScriptSystem>
{
    private:
        /**
         * The script systems internal lua state that handles all
         * interactions between C and Lua
         */
        entityx::EventManager events;
        entityx::EntityManager manager;

        sol::state lua;

    public:
        ScriptSystem(void)
            : manager(events){}

        ~ScriptSystem(void)
        {

        }

        int init(void)
        {
            lua.open_libraries(sol::lib::base);
            scriptExport();
            doFile();

            //Script::CreateNewState();

            return 0;
        }

        void configure([[maybe_unused]]entityx::EntityManager& entities,
                       [[maybe_unused]]entityx::EventManager& events) final
        {
            //manager = std::make_shared<entityx::EntityManager>(std::move(entities));
            
            events.subscribe<EntitySpawnEvent>(*this);

            init();
        }
        
        void update([[maybe_unused]] entityx::EntityManager& entites,
                    [[maybe_unused]] entityx::EventManager& events,
                    [[maybe_unused]] entityx::TimeDelta dt) final
        {

        }

        void doFile(void)
        {
            auto result = lua.script_file("Scripts/init.lua");
            if (!result.valid()) {
                std::cout << "Lua error: " << std::endl;
            }

            manager.each<Position>(
                    [&](entityx::Entity, Position& p)
                    {std::cout << p.x << "," << p.y << std::endl;});
        }

        sol::table spawn([[maybe_unused]]sol::object param)
        {
            sol::table toRet = lua.create_table_with();

            //lb::LuaRef entity(state.get());
            //entity = lb::newTable(state.get());
            
            if (param.get_type() == sol::type::table) {
                sol::table tab = param;

                entityx::Entity e = manager.create();

                if (tab["Position"] != nullptr) {
                    toRet["Position"] = 
                        e.assign<Position>(Position().FromLua(tab["Position"])).get();
                }

                if (tab["init"] != nullptr) {
                    toRet["init"] = tab["init"];
                }

            } else {
                std::cerr << "Parameter to spawn() must be a table!" << std::endl;
            }

            //return entity;
            return toRet;
        }

        void scriptExport()
        {

            std::function<sol::table(sol::table)> func = 
                [this](sol::table t){ return spawn(t);};

            lua.new_usertype<Position>("Position",
                    sol::constructors<Position(float x, float y), Position()>(),
                    "x", &Position::x,
                    "y", &Position::y);

            auto gamespace = lua["game"].get_or_create<sol::table>();
            gamespace.set_function("spawn", func);
        }

        void receive (const EntitySpawnEvent &toSpawn)
        {
            //toSpawn.ret = spawn(toSpawn.ref);
            (void)toSpawn;
        }
};

#endif//SCRIPT_HPP_
