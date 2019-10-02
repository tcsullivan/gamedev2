/**
 * @file engine.cpp
 * Manages all game systems.
 *
 * Copyright (C) 2019 Clyne Sullivan
 * Copyright (C) 2019 Belle-Isle, Andrew <drumsetmonkey@gmail.com>
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

#include "audio.hpp"
#include "config.hpp"
#include "engine.hpp"
#include "gamestate.hpp"
#include "gamerun.hpp"
#include "input.hpp"
#include "player.hpp"
#include "script.hpp"
#include "render.hpp"
#include "physics.hpp"
#include "text.hpp"

#include "components/EventListener.hpp"
#include "components/Script.hpp"
#include "components/Position.hpp"
#include "components/Velocity.hpp"

#include <fstream>

using namespace std::literals::string_literals;
using namespace std::chrono_literals;
namespace cr = std::chrono;
typedef std::chrono::high_resolution_clock mc;

int Engine::init(void)
{
    Config::load();

    systems.add<GameRunSystem>();
    systems.add<InputSystem>();
    systems.add<PlayerSystem>(entities);
    systems.add<WorldSystem>();
    systems.add<RenderSystem>();
    systems.add<ScriptSystem>(entities, *(systems.system<WorldSystem>().get()));
    systems.add<PhysicsSystem>();
    systems.add<TextSystem>();
    systems.add<AudioSystem>();
    systems.configure();

    // Load game script and entity data
    auto* script = systems.system<ScriptSystem>().get();
    script->addToGameNamespace("loadFont",
        bindInstance(&TextSystem::loadFont,
                     systems.system<TextSystem>().get()));
    script->addToGameNamespace("puts",
        bindInstance(&TextSystem::put,
                     systems.system<TextSystem>().get()));
    script->init();
    

    if (GameState::load("save.json", entities)) {
        std::cout << "Loaded from save.json. Delete the file if you don't want "
                     "it." << std::endl;
    }

    // Initially update the world to send all systems world data
    systems.update<WorldSystem>(0);
    return 0;
}

void Engine::logicLoop(void)
{
    entityx::TimeDelta dt = 0; /**< Elapsed milliseconds since each loop */
    double elapsed = 1000;     /**< Time elapsed since last logic loop. This
                                    should be initialized to something larger
                                    than our logic loop period (50ms), so
                                    the logic loop is run during our first
                                    loop. */

    auto start = mc::now();
    while (shouldRun()) {
        auto end = start;
        start = mc::now();
        auto diff = start-end;
        auto micros = cr::duration_cast<cr::microseconds>(diff);
        auto msc = micros.count();
        dt = static_cast<double>(msc) / 1000.0;
        elapsed += dt;

        systems.update<InputSystem>(dt);
        //systems.update<ScriptSystem>(dt);

        /*******************
        *  LOGIC UPDATES  *
        *******************/
        
        // Update 20 times a second
        if (elapsed > 50) {
            elapsed = 0;

            systems.update<WorldSystem>(dt);
            
            // All entities with an idle function should be run here
            entities.each<Scripted>([](entityx::Entity, Scripted &f){
                f.exec();
            });
        }

        std::this_thread::yield();
    }
}

void Engine::physicsLoop(void)
{
    entityx::TimeDelta dt = 0; /**< Elapsed milliseconds since each loop */
    auto start = mc::now();
    while (shouldRun()) {
        auto end = start;
        start = mc::now();

        auto diff = start - end;
        auto micros = cr::duration_cast<cr::microseconds>(diff);
        auto msc = micros.count();
        dt = static_cast<double>(msc) / 1000.0;

        // Update the entities physics/position
        systems.update<PhysicsSystem>(dt);

        std::this_thread::yield();
    }
}

void Engine::renderLoop(int& fpsCounter)
{
    entityx::TimeDelta dt = 0; /**< Elapsed milliseconds since each loop */
    while (shouldRun()) {
        systems.update<TextSystem>(dt);
        systems.update<RenderSystem>(dt);
        fpsCounter++;
    }
}

void Engine::run(void)
{
    int fpsCounter = 0;

    // Start logic thread
    logicThread = std::thread([this](void) {
        logicLoop();
    });

    physicsThread = std::thread([this](void) {
        physicsLoop();
    });

    debugThread = std::thread([this, &fpsCounter](void) {
        while (shouldRun()) {
            std::this_thread::sleep_for(1s);
            fps = fpsCounter;
            fpsCounter = 0;
            systems.system<TextSystem>()->put("default", 0, 0, "fps: "s + std::to_string(fps));
        }
    });

    // Keep render loop on main thread
    renderLoop(fpsCounter);

    // Done, bring logic thread back
    logicThread.join();
    physicsThread.join();
    debugThread.join();

    // Save the entities' data, and settings data
    GameState::save("save.json", entities);
    Config::save();

    // Remove all Lua references from entities
    entities.each<Scripted>([](entityx::Entity, Scripted &f) { 
        f.cleanup(); });
    entities.each<EventListener>([](entityx::Entity, EventListener &f) {
        f.cleanup(); });
    systems.system<WorldSystem>()->cleanup();
}

bool Engine::shouldRun(void)
{
    auto grs = systems.system<GameRunSystem>();
    return grs ? grs->shouldRun() : true;
}

