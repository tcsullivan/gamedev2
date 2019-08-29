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

#include <SDL2/SDL.h>

#include <atomic>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

#include <components/Position.hpp>
#include <script.hpp>

class Window {
private:
	constexpr static const char *title = "gamedev2";
	constexpr static int width = 640;
	constexpr static int height = 480;

	static std::unique_ptr<SDL_Window, void (*)(SDL_Window *)> window;
	static SDL_GLContext context;

	static void destroyWindow(SDL_Window *w) {
		SDL_GL_DeleteContext(context);
		SDL_DestroyWindow(w);
	}

public:
	static int init(void) {
		if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
			std::cerr << "SDL video failed to initialize: "
				<< SDL_GetError() << std::endl;
			return -1;
		}

		window.reset(SDL_CreateWindow(title,
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			width, height,
			SDL_WINDOW_OPENGL));

		if (window.get() == nullptr) {
			std::cerr << "SDL window creation failed: "
				<< SDL_GetError() << std::endl;
			return -1;
		}

		context = SDL_GL_CreateContext(window.get());

		return 0;
	}

	static void render(void) {
		SDL_GL_SwapWindow(window.get());
	}
};

std::unique_ptr<SDL_Window, void (*)(SDL_Window *)> Window::window (nullptr,
	Window::destroyWindow);
SDL_GLContext Window::context;

std::atomic_bool shouldRun;

static void renderLoop(void);
static void logicLoop(void);
static void LuaTest(void);

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

    LuaTest();

	// Create our window
	Window::init();

	// Start game
	shouldRun.store(true);
	std::thread logic (logicLoop);
	renderLoop();
	logic.join();

	return 0;
}

void renderLoop(void)
{
	while (shouldRun.load()) {
		Window::render();
		std::this_thread::yield();
	}
}

void logicLoop(void)
{
	using namespace std::chrono_literals;

	std::cout << "Press escape to exit." << std::endl;

	while (shouldRun.load()) {
		for (SDL_Event event; SDL_PollEvent(&event);) {
			switch (event.type) {
			case SDL_KEYUP:
				// Exit game on escape
				if (event.key.keysym.sym == SDLK_ESCAPE)
					shouldRun.store(false);
				break;
			default:
				break;
			}
		}

		std::this_thread::sleep_for(100ms);
	}
}

//struct Position : entityx::Component<Position>
//{
//    Position(float _x, float _y): x(_x), y(_y) {}
//    Position(void){x = y = 0.0;}
//    
//    float x,y;
//};

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
