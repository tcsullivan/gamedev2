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

#include <soil/SOIL.h>

#include "texture.hpp"
#include "events/render.hpp"

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

class Layer
{
    friend class World;
private:
    Texture texture;
    Texture normal;

    float drawLayer = 0.0f;

    GLuint layerVBO;
public:

    Layer(float z, sol::table tab) {
        drawLayer = z;
        if (tab["texture"] != nullptr) {
            sol::object t = tab["texture"];
            texture = Texture(t);
        }
        if (tab["normal"] != nullptr) {
            sol::object n = tab["normal"];
            normal = Texture(n);
        }

        glGenBuffers(1, &layerVBO);
    }
};

class SolidLayer : public Layer
{
    friend class World;
private:
    std::vector<std::vector<bool>> hitbox;
public:
    SolidLayer(float z, sol::table tab) : Layer(z, tab) {
        if (tab["hitbox"] != nullptr) {
            int width, height;
            unsigned char* box = 
                SOIL_load_image(std::string(tab["hitbox"]).c_str(),
                                &width, &height, 0,
                                SOIL_LOAD_RGBA);

            for (int w = 0; w < width; w++) {
                hitbox.push_back(std::vector<bool>(height));
                for (int h = 0; h < height; h++) {
                    unsigned char* c = &box[(h) + (height*w*4)];
                    // we want to read the red channel
                    if (c[3]) {
                        hitbox[w][h] = true;
                    }
                    else
                        hitbox[w][h] = false;
                }
            }

            SOIL_free_image_data(box);
        }
    }
};

class World
{
    friend class WorldSystem;
private:
    unsigned int seed;
    unsigned int layers;

    unsigned int unitSize;
    std::vector<SolidLayer> solidLayers;
    std::vector<Layer> drawLayers;

protected:
    // RENDER
    std::vector<WorldMeshUpdateEvent> meshAdd;
    GLuint worldVBO;
public:
    /* VARS */
    sol::function generate;
    sol::function registerMat;

    World() {}
    World(sol::object ref);
    ~World() {
        registerMat = sol::nil;
        generate = sol::nil;
    }

    std::tuple<unsigned int, unsigned int, unsigned int> getSize();

    /* RENDERING */
    void generateMesh();

    /* SEED */
    unsigned int getSeed();
    unsigned int setSeed(unsigned int);

    /* PHYSICS */
    double getHeight(double x, double y, double z);

    // NEW
    unsigned int getUnitSize() {return unitSize;}
    void setUnitSize(unsigned int u) {unitSize = u;}

    void registerLayer(float, sol::object);
    void registerDecoLayer(float, sol::object);
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
