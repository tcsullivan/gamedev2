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
    unsigned int seed;
    unsigned int layers;

    std::vector<std::vector<std::vector<unsigned int>>> data;
    std::vector<unsigned int> registry;
public:
    World() {}
    World(sol::object ref);
    ~World() {
        generate = sol::nil;
        registry.clear();
        data.clear();
    }

    sol::function generate;

    /* SEED */
    unsigned int getSeed() {return seed;}
    void setSeed(unsigned int s) {seed = s;}

    /* LAYERS */
    unsigned int getLayers() {return layers;}
    // If we change the amount of layers, we have to regenerate the world
    void setLayers(unsigned int l) {
        layers = l;
        generate();
    }

    /* DATA */
    void setData(unsigned int x, 
                 unsigned int y,
                 unsigned int z,
                 unsigned int d);
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

    ~WorldSystem(void) {
        currentWorld = nullptr;
        worlds.clear();
    }

    World* addWorld(sol::object);
    void cleanup()
    {
        worlds.clear();
    }

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

