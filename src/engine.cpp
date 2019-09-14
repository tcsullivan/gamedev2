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

#include "engine.hpp"
#include "gamestate.hpp"
#include "gamerun.hpp"
#include "input.hpp"
#include "player.hpp"
#include "script.hpp"
#include "render.hpp"
#include "physics.hpp"

#include "components/EventListener.hpp"
#include "components/Script.hpp"
#include "components/Position.hpp"
#include "components/Velocity.hpp"

#include <fstream>

using namespace std::chrono_literals;
namespace cr = std::chrono;
typedef std::chrono::high_resolution_clock mc;

int Engine::init(void)
{
    systems.add<GameRunSystem>();
    systems.add<InputSystem>();
    systems.add<PlayerSystem>(entities);
    systems.add<WorldSystem>();
    systems.add<RenderSystem>(*(systems.system<WorldSystem>().get()));
    systems.add<ScriptSystem>(entities, *(systems.system<WorldSystem>().get()));
    systems.add<PhysicsSystem>();
    systems.configure();

    // Load game script and entity data
    systems.system<ScriptSystem>()->init();
    if (GameState::load("save.json", entities)) {
        std::cout << "Loaded from save.json. Delete the file if you don't want "
                     "it." << std::endl;
    }

    return 0;
}

void Engine::logicLoop(void)
{
    entityx::TimeDelta dt = 0; /**< Elapsed milliseconds since each loop */
    double elapsed = 0;

    while (shouldRun()) {
        auto start = mc::now();

        systems.update<InputSystem>(dt);
        //systems.update<ScriptSystem>(dt);

        /*******************
        *  LOGIC UPDATES  *
        *******************/
        
        // Update 20 times a second
        if (elapsed > 50) {
            elapsed = 0;
            
            // All entities with an idle function should be run here
            entities.each<Scripted>([](entityx::Entity, Scripted &f){
                f.exec();
            });
        }

        std::this_thread::yield();

        auto end = mc::now();
        auto diff = end - start;
        auto micros = cr::duration_cast<cr::microseconds>(diff);
        auto msc = micros.count();
        dt = static_cast<double>(msc) / 1000.0;
        elapsed += dt;
    }
}

void Engine::physicsLoop(void)
{
    entityx::TimeDelta dt = 0; /**< Elapsed milliseconds since each loop */

    while (shouldRun()) {
        auto start = mc::now();

        // Update the entities physics/position
        systems.update<PhysicsSystem>(dt);

        std::this_thread::yield();

        auto end = mc::now();
        auto diff = end - start;
        auto micros = cr::duration_cast<cr::microseconds>(diff);
        auto msc = micros.count();
        dt = static_cast<double>(msc) / 1000.0;
    }
    std::cout << std::endl;
}

void Engine::renderLoop(void)
{
    entityx::TimeDelta dt = 0; /**< Elapsed milliseconds since each loop */
    while (shouldRun()) {
        systems.update<RenderSystem>(dt);
    }
}

void Engine::run(void)
{
    // Start logic thread
    logicThread = std::thread([this](void) {
        logicLoop();
    });

    physicsThread = std::thread([this](void) {
        physicsLoop();
    });

    // Keep render loop on main thread
    renderLoop();

    // Done, bring logic thread back
    logicThread.join();
    physicsThread.join();

    // Save the entities' data
    GameState::save("save.json", entities);

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

