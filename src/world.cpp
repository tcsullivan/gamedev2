/**
 * @file world.cpp
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

#include "world.hpp"

#include "events/world.hpp"

/*****************
*  WORLD CLASS  *
*****************/
/* CONSTRUCTORS */
World::World(sol::object param)
{
    if (param.get_type() == sol::type::table) {
        sol::table tab = param;

        if (tab["Seed"] == sol::type::number) {
            seed = tab["Seed"];
        }
        if (tab["Layers"] == sol::type::number) {
            layers = tab["Layers"];
        }
        if (tab["Register"] == sol::type::function) {
            registerMat = tab["Register"];
        }
        if (tab["Generate"] == sol::type::function) {
            generate = tab["Generate"];
        }

    } else {
        // TODO better logging
        std::cerr << "World paramaters must be stored in a table" << std::endl;
    }

    // If there is a register function, we should call it here
    if (registerMat != sol::nil)
        registerMat(this);

    // If a generate function is defined, call it
    if (generate != sol::nil)
        generate(this);
}

// TODO
std::tuple<unsigned int, unsigned int, unsigned int>
World::getSize()
{
    //return {width, height, layers};
    return {0, 0, 0};
}

/* RENDERING */
void World::generateMesh()
{
    for (auto &l : drawLayers) {
        
        // Preallocate size of vertexes

        float Z = l->drawLayer;
        auto to = l->texture.offset;
        auto ts = l->texture.size;
        float tr = 1.0f;

        float w = l->texture.width/unitSize;
        float h = l->texture.height/unitSize;

        GLfloat mesh[36] = {0  , 0  , Z, to.x     , to.y+ts.y, tr,
                            0+w, 0  , Z, to.x+ts.x, to.y+ts.y, tr,
                            0  , 0+h, Z, to.x     , to.y     , tr,

                            0+w, 0  , Z, to.x+ts.x, to.y+ts.y, tr,
                            0+w, 0+h, Z, to.x+ts.x, to.y     , tr,
                            0  , 0+h, Z, to.x     , to.y     , tr};

        glBindBuffer(GL_ARRAY_BUFFER, l->layerVBO);
        glBufferData(GL_ARRAY_BUFFER, 
                     36 * sizeof(GLfloat), 
                     mesh, 
                     GL_STATIC_DRAW);

        meshAdd.push_back(WorldMeshUpdateEvent(l->layerVBO,
                                               l->texture.tex,
                                               l->normal.tex,
                                               36));
    }
}

/* SEED */
unsigned int World::getSeed()
{
    return seed;
}

unsigned int World::setSeed(unsigned int s)
{
    seed = s;
    return seed;
}

/* PHYSICS */
double World::getHeight(double x, double y, double z)
{
    (void)y;
    double Y = 0.0f;
    for (auto &l : solidLayers) {
        if (z == l->drawLayer) {
            int wx = x*unitSize;

            int h = 0.0;
            for (auto b : l->hitbox[wx]) {
                if (b == true)
                    Y = h;
                h++;
            }
            return ((Y+1)/unitSize);
        }
    }
    return 0;
}

std::vector<std::pair<glm::vec2, glm::vec2>> 
World::getIntersectingPlanes(glm::vec2 origin, glm::vec2 dest)
{
    (void)origin;
    (void)dest;
    //glm::ivec2 worldOrigin = origin*unitSize;
    //glm::ivec2 worldDest = dest*unitSize;

    return std::vector<std::pair<glm::vec2, glm::vec2>>();
}

glm::vec3 World::collide(glm::vec3 &start, glm::vec3 &end, Physics &phys)
{
    (void)start;
    (void)end;
    (void)phys;
    for (auto &l : solidLayers) {
        if (end.z == l->drawLayer) {
            glm::vec2 len = end-start;
            glm::vec2 dir = glm::normalize(len);
            float step = 1.0f/unitSize;

            // TODO move this
            glm::vec2 pos = start;

            for (float i = 0; i < len.length(); i+=step) {
                pos += dir;

                if (dir.x > 0.0f) {
                // Moving to the right
                    //glm::vec2 origin = pos + phys.corners[1]; // bottom right
                    //glm::vec2 orDir = glm::vec2(0, 1);

                } else if (dir.x < 0.0f) {
                // Moving to the left

                }

                if (dir.y > 0.0f) {
                // Moving upwards

                } else if (dir.y < 0.0f) {
                // Moving downwards

                }
            }
        }
    }
    return glm::vec3(0);
}


/*********
*  NEW  *
*********/
void World::registerLayer(float z, sol::object obj)
{
    if (obj.get_type() == sol::type::table) {
        sol::table tab = obj;
        SolidLayer s(z, tab);
        solidLayers.push_back(std::make_shared<SolidLayer>(s));
        drawLayers.push_back(std::make_shared<Layer>(s));
    } else {
        throw std::string("Layer must receive a table");
    }
    generateMesh();
}

void World::registerDecoLayer(float z, sol::object obj)
{
    if (obj.get_type() == sol::type::table) {
        sol::table tab = obj;
        drawLayers.push_back(std::make_shared<Layer>(Layer(z, tab)));
    } else {
        throw std::string("Layer must receive a table");
    }
    generateMesh();
}


/******************
*  WORLD SYSTEM  *
******************/

World* WorldSystem::addWorld(sol::object t)
{
    worlds.push_back(World(t));
    return &(worlds.back());
}

void WorldSystem::configure([[maybe_unused]]entityx::EntityManager& entities,
                            [[maybe_unused]]entityx::EventManager& events)
{

}

void WorldSystem::update([[maybe_unused]]entityx::EntityManager& entities,
                         [[maybe_unused]]entityx::EventManager& events,
                         [[maybe_unused]]entityx::TimeDelta dt)
{
    if (currentWorld == nullptr) {
        currentWorld = &(worlds.front());
        events.emit<WorldChangeEvent>(currentWorld);
    }

    for (auto &ma : currentWorld->meshAdd) {
        events.emit<WorldMeshUpdateEvent>(ma);
    }
    currentWorld->meshAdd.clear();
}
