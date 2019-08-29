/**
 * @file main.cpp
 * The main file, where the magic begins.
 *
 * Copyright (C) 2019 Clyne Sullivan
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <lua.hpp>
#include <entityx/entityx.h>
#include <LuaBridge/LuaBridge.h>

#include "engine.hpp"

#include <SDL2/SDL.h>

#include <iostream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
	// Initialize SDL
	if (SDL_Init(0) != 0) {
		std::cerr << "SDL failed to initialize: " << SDL_GetError()
			<< std::endl;
		return -1;
	} else {
		atexit(SDL_Quit);
	}

    //LuaTest();

	// Create the engine
	Engine engine;
	engine.init();

	// Go go go!
	engine.run();

	return 0;
}

/*
using namespace entityx;
namespace lb = luabridge;

EventManager events;
EntityManager entities(events);

lua_State* L;

//lb::LuaRef spawn(lb::LuaRef ref)
//{
//    lb::LuaRef entity(L);
//    entity = lb::newTable(L);
//    
//    if (ref.isTable()) {
//
//        Entity e = entities.create();
//
//        for (auto &&comp : lb::pairs(ref)) {
//            if (comp.first.cast<std::string>() == "Position") {
//                entity["Position"] = 
//                    e.assign<Position>(Position().FromLua(comp.second)).get();
//            } else if (comp.first.cast<std::string>() == "init") {
//                entity["init"] = comp.second;
//            }
//        }
//    } else {
//        std::cerr << "Parameter to spawn() must be a table!" << std::endl;
//    }
//
//    return entity;
//}
//

ScriptSystem sc;

lb::LuaRef spawn(lb::LuaRef ref)
{
    return sc.spawn(ref);
}


void LuaTest(void)
{
    
    sc.configure(entities, events);
    
    // Functions export
    lb::getGlobalNamespace(sc.getState())
        .beginNamespace("game")
            .addFunction("spawn", spawn)
        .endNamespace();

    sc.doFile();


    //L = luaL_newstate();
    //luaL_openlibs(L);

    //lb::getGlobalNamespace(L).
    //    beginNamespace("comp")
    //        .beginClass<Position>("Position")
    //            .addConstructor<void(*)(float, float)>()
    //            .addProperty("x", &Position::x)
    //            .addProperty("y", &Position::y)
    //        .endClass()
    //    .endNamespace();

    //lb::getGlobalNamespace(L)
    //    .beginNamespace("game")
    //        .addFunction("spawn", spawn)
    //    .endNamespace();

    //if (luaL_dofile(L, "Scripts/init.lua")) {
    //    std::cout << "Lua error: " << lua_tostring(L, -1) << std::endl;
    //}

    //entities.each<Position>([&](Entity, Position& p){std::cout << p.x << "," << p.y << std::endl;});

    //lua_close(L);
}
*/
