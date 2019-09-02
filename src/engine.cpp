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
#include "gamerun.hpp"
#include "input.hpp"
#include "player.hpp"
#include "script.hpp"
#include "render.hpp"

#include "components/Script.hpp"
#include "components/Position.hpp"
#include "components/Velocity.hpp"

using namespace std::chrono_literals;
namespace cr = std::chrono;
typedef std::chrono::high_resolution_clock mc;

int Engine::init(void)
{
    systems.add<GameRunSystem>();
    systems.add<InputSystem>();
    systems.add<PlayerSystem>();
    systems.add<RenderSystem>();
    systems.add<ScriptSystem>();
    systems.configure();

    systems.system<ScriptSystem>()->init();

    return 0;
}

void Engine::logicLoop(void)
{
    entityx::TimeDelta dt = 0; /**< Elapsed milliseconds since each loop */
    double elapsed = 0;

    while (shouldRun()) {
        auto start = mc::now();

        /***********************
        *  UPDATE FREQUENTLY  *
        ***********************/
        
        entities.each<Position, Velocity>
            ([&](entityx::Entity, Position &p, Velocity &v){
            p.x += (v.x * dt/1000.0);
            p.y += (v.y * dt/1000.0);
        });

        systems.update<InputSystem>(dt);
        systems.update<ScriptSystem>(dt);

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

        auto end = mc::now();
        auto diff = end - start;
        auto micros = cr::duration_cast<cr::microseconds>(diff);
        auto msc = micros.count();
        dt = static_cast<double>(msc) / 1000.0;
        elapsed += dt;
        std::this_thread::yield();
    }

    // Remove all Lua references from entities
    entities.each<Scripted>([](entityx::Entity, Scripted &f){ f.cleanup(); });
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

    // Keep render loop on main thread
    renderLoop();

    // Done, bring logic thread back
    logicThread.join();
}

bool Engine::shouldRun(void)
{
    auto grs = systems.system<GameRunSystem>();
    return grs ? grs->shouldRun() : true;
}

