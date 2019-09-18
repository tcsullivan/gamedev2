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
#include "events/render.hpp"
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

    // Create our world VBO
    glGenBuffers(1, &worldVBO);
    // Generate our mesh
    generateMesh();
}

/* REGISTRY */
void World::registerMaterial(std::string name, sol::object data)
{
    if (data.get_type() == sol::type::table) {
        sol::table tab = data;

        // Make sure this material has not been registered before
        auto it = string_registry.find(name);
        if (it == string_registry.end()) {
            string_registry.emplace(name, registry.size());
            registry.push_back(WorldMaterial(tab));
        } else {
            std::cerr << "Material: " << name 
                      << " was already registered" << std::endl;
        }
    } else {
        // TODO better logging
        std::cerr << "Material registration must have a table" << std::endl;
    }
}

/* DATA */
void World::setData(unsigned int x,
                    unsigned int y,
                    unsigned int z,
                    std::string d)
{
    unsigned int discovered = -1;

    auto found = string_registry.find(d);
    if (found != string_registry.end())
        discovered = found->second;

    try {
        data.at(z).at(x).at(y) = discovered;
    } catch (std::out_of_range &oor) {
    // Make sure any assignments that are outsize specified world size are
    //  caught to avoid any seg faults
        std::cerr << "Unable to set data at: "
                  << x << "," << y << "," << z
                  << " Exception: " << oor.what() << std::endl;
    }
}

/* SIZE */
std::tuple<unsigned int, unsigned int, unsigned int>
World::setSize(unsigned int x, unsigned int y, unsigned int z)
{
    width = x;
    height = y;
    layers = z;

    data = std::vector<std::vector<std::vector<int>>>
        (z, std::vector<std::vector<int>>
            (x,std::vector<int>
                (y, -1)
            )
        );

    return {width, height, layers};
}

std::tuple<unsigned int, unsigned int, unsigned int>
World::getSize()
{
    return {width, height, layers};
}

/* RENDERING */
void World::generateMesh()
{
    //const unsigned int voxelLength = 6; // 2 triangles @ 3 points each

    // Preallocate size of vertexes
    mesh = std::basic_string<WorldMeshData>();
    for (float Z = 0; Z < data.size(); Z++) {
        for (float X = 0; X < data.at(Z).size(); X++) {
            for (float Y = 0; Y < data.at(Z).at(X).size(); Y++) {
                int d = data.at(Z).at(X).at(Y);

                if (d == -1) // Don't make a mesh for air of course
                    continue;

                Texture &t = registry.at(d).texture;
                glm::vec2& to = t.offset;
                glm::vec2& ts = t.size;

                mesh += {X  , Y  , Z, to.x     , to.y+ts.y, 1.0};
                mesh += {X+1, Y  , Z, to.x+ts.x, to.y+ts.y, 1.0};
                mesh += {X  , Y+1, Z, to.x     , to.y     , 1.0};

                mesh += {X+1, Y  , Z, to.x+ts.x, to.y+ts.y, 1.0};
                mesh += {X+1, Y+1, Z, to.x+ts.x, to.y     , 1.0};
                mesh += {X  , Y+1, Z, to.x     , to.y     , 1.0};
            }
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, worldVBO);
    glBufferData(GL_ARRAY_BUFFER, 
                 mesh.size() * sizeof(mesh), 
                 &mesh.front(), 
                 GL_STREAM_DRAW);

    meshUpdated = true;
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
    unsigned int X = static_cast<unsigned int>(x);
    unsigned int Z = static_cast<unsigned int>(z);

    double Y = 0.0;
    try {
        auto &d = data.at(Z).at(X);
        for (int yi = d.size()-1; yi >= 0; yi--) {
            if (d.at(yi) >= 0) {
                if (!registry.at(d.at(yi)).passable) {
                    Y = static_cast<double>(yi);
                    Y += 1;
                    break;
                }
            }
        }
    } catch (...) { // If we get any errors, just let the character 
        return y;
    }

    return Y;
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
        std::cout << "Emitted" << std::endl;
    }

    if (currentWorld->meshUpdated) {
        events.emit<NewRenderEvent>(
            currentWorld->worldVBO, 
            currentWorld->getTexture(),
            currentWorld->getNormal(),
            currentWorld->mesh.size()
        );
    }
}
