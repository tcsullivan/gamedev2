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
#include "script.hpp"
#include "render.hpp"

#include "components/Script.hpp"
#include "components/Position.hpp"

int Engine::init(void)
{
	systems.add<GameRunSystem>();
	systems.add<InputSystem>();
    systems.add<RenderSystem>();
    systems.add<ScriptSystem>();

	systems.configure();
	return 0;
}

void Engine::logicLoop(void)
{
	using namespace std::chrono_literals;

	entityx::TimeDelta dt = 0;

	while (shouldRun()) {
		systems.update<InputSystem>(dt);

        // All entities with an idle function should be run here
        entities.each<Scripted>([](entityx::Entity, Scripted &f){
            f.exec();
        });
		std::this_thread::sleep_for(100ms);
	}

    // Remove all Lua references from entities
    entities.each<Scripted>([](entityx::Entity, Scripted &f){ f.cleanup(); });
}

void Engine::renderLoop(void)
{
	while (shouldRun()) {
		systems.update<RenderSystem>(0);
		std::this_thread::yield();
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

