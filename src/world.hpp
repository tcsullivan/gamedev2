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

struct WorldMeshData
{
    float posX, posY, posZ;
    float texX, texY;
    float transparency;
}__attribute__((packed));

struct WorldMaterial
{
    bool passable = false;

    Texture texture;
    Texture normal;

    WorldMaterial(sol::table tab) {
        if (tab["texture"] != nullptr) {
            sol::object t = tab["texture"];
            texture = Texture(t);
        }
        if (tab["normal"] != nullptr) {
            sol::object n = tab["normal"];
            normal = Texture(n);
        }
        if (tab["passable"] == sol::type::boolean) {
            passable = tab["passable"];
        }
    }
};

class World
{
    friend class WorldSystem;
private:
    unsigned int seed;
    unsigned int layers;

    unsigned int height;
    unsigned int width;

    std::vector<std::vector<std::vector<int>>> data;

    std::unordered_map<std::string, int> string_registry;
    std::vector<WorldMaterial> registry;

protected:
    // RENDER
    std::basic_string<WorldMeshData> mesh;
    GLuint worldVBO;
    bool meshUpdated = false;
public:
    /* VARS */
    sol::function generate;
    sol::function registerMat;

    World() {}
    World(sol::object ref);
    ~World() {
        registerMat = sol::nil;
        generate = sol::nil;
        registry.clear();
        data.clear();
    }

    /* REGISTRY */
    void registerMaterial(std::string, sol::object);

    /* DATA */
    void setData(unsigned int, unsigned int, unsigned int, std::string);

    /* SIZE */
    std::tuple<unsigned int, unsigned int, unsigned int> setSize(unsigned int,
                                                                 unsigned int,
                                                                 unsigned int);
    std::tuple<unsigned int, unsigned int, unsigned int> getSize();

    /* RENDERING */
    void generateMesh();
    std::basic_string<WorldMeshData>& getMesh() {return mesh;}
    GLuint getTexture() {return registry.at(0).texture.tex;}
    GLuint getNormal() {return registry.at(0).normal.tex;};

    /* SEED */
    unsigned int getSeed();
    unsigned int setSeed(unsigned int);

    /* PHYSICS */
    double getHeight(double x, double y, double z);
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
    World* current() {return currentWorld;};
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
