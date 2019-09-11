/**
 * @file world.hpp
 * Manages the world systems
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

#ifndef SYSTEM_WORLD_HPP_
#define SYSTEM_WORLD_HPP_

#include <vector>

#include <entityx/entityx.h>
#include <sol/sol.hpp>

#include "texture.hpp"

struct WorldMaterial
{
    Texture texture;
    Texture normal;
};

class World
{
private:
    std::vector<unsigned int> registry;
    std::vector<std::vector<unsigned int>> worldData;
public:
    World(sol::object ref);
    ~World() {}
};

/**
 * @class WorldSystem
 * Handles the game's world system
 */
class WorldSystem : public entityx::System<WorldSystem>
{
private:
    std::vector<World> worlds;
    World* currentWorld;
public:
    WorldSystem(void):
        currentWorld(nullptr) {}

    ~WorldSystem(void) {}

    World* addWorld(sol::object);

    /**
     * Prepares the system for running.
     */
    void configure(entityx::EntityManager& entities,
                   entityx::EventManager& events) final;
    
    /**
     * Updates the world ticks (entity spawns and world events)
     */
    void update(entityx::EntityManager& entites,
                entityx::EventManager& events,
                entityx::TimeDelta dt) final;
};

#endif // SYSTEM_WORLD_HPP_

