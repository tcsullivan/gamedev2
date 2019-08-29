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
#include <LuaBridge/LuaBridge.h>
#include <script/script.hpp>

/****************
*  COMPONENTS  *
****************/
#include <components/Position.hpp>

namespace lb = luabridge;

struct EntitySpawnEvent {
    EntitySpawnEvent (lb::LuaRef ref)
        : ref(ref), ret(nullptr){}

    lb::LuaRef ref;
    lb::LuaRef ret;
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
        std::unique_ptr<lua_State, void(*)(lua_State *)> state;
        entityx::EventManager events;
        entityx::EntityManager manager;

    public:
        ScriptSystem(void)
            : state(luaL_newstate(), lua_close),
              manager(events){}

        ~ScriptSystem(void)
        {

        }

        lua_State* getState()
        {
            return state.get();
        }

        int init(void)
        {
            luaL_openlibs(state.get());
            scriptExport();
            //doFile();

            Script::CreateNewState();

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
            if (luaL_dofile(state.get(), "Scripts/init.lua")) {
                std::cout << "Lua error: " << lua_tostring(state.get(), -1) << std::endl;
            }

            manager.each<Position>(
                    [&](entityx::Entity, Position& p)
                    {std::cout << p.x << "," << p.y << std::endl;});
        }

        lb::LuaRef spawn(lb::LuaRef ref)
        {
            lb::LuaRef entity(state.get());
            entity = lb::newTable(state.get());
            
            if (ref.isTable()) {

                entityx::Entity e = manager.create();

                for (auto &&comp : lb::pairs(ref)) {
                    if (comp.first.cast<std::string>() == "Position") {
                        entity["Position"] = 
                            e.assign<Position>(Position().FromLua(comp.second)).get();
                    } else if (comp.first.cast<std::string>() == "init") {
                        entity["init"] = comp.second;
                    }
                }
            } else {
                std::cerr << "Parameter to spawn() must be a table!" << std::endl;
            }

            return entity;
        }

        void scriptExport()
        {
            // Components export
            lb::getGlobalNamespace(state.get()).
                beginNamespace("comp")
                    .beginClass<Position>("Position")
                        .addConstructor<void(*)(float, float)>()
                        .addProperty("x", &Position::x)
                        .addProperty("y", &Position::y)
                    .endClass()
                .endNamespace();

            lb::getGlobalNamespace(state.get())
                .beginNamespace("game")
                .endNamespace();

            // Functions export
            //lb::getGlobalNamespace(state.get())
            //    .beginNamespace("game")
            //        .addFunction("spawn", &ScriptSystem::spawn)
            //    .endNamespace();
        }

        void receive (const EntitySpawnEvent &toSpawn)
        {
            //toSpawn.ret = spawn(toSpawn.ref);
            (void)toSpawn;
        }
};

#endif//SCRIPT_HPP_
