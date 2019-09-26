/**
 * @file window.hpp
 * Manages all game systems.
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

#ifndef ENGINE_HPP_
#define ENGINE_HPP_

#include <entityx/entityx.h>

#include <thread>

/**
 * @class Engine
 * Manages all game entities, events, and systems.
 */
class Engine
{
private:
    entityx::EventManager events;
    entityx::EntityManager entities;
    entityx::SystemManager systems;

    int fps;

    std::thread logicThread;
    std::thread physicsThread;
    std::thread debugThread;

    void logicLoop(void);
    void physicsLoop(void);
    void renderLoop(int& fpsCounter);

    bool shouldRun(void);

public:
    Engine(void) :
        entities(events),
        systems(entities, events),
        fps(0) {}

    /**
     * Initializes the game engine.
     * @return Zero on success, non-zero otherwise
     */
    int init(void);

    /**
     * Runs the game engine.
     * Function returns when game is told to end/exit.
     */
    void run(void);
};

#endif // ENGINE_HPP_

