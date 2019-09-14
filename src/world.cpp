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

    if (registerMat != sol::nil)
        registerMat(this);
    if (generate != sol::nil)
        generate(this);
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

    data = std::vector<std::vector<std::vector<unsigned int>>>
        (z, std::vector<std::vector<unsigned int>>
            (x,std::vector<unsigned int>
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
    const unsigned int coordLength = 3 + // x, y, z
                                     2 + // texture coords
                                     1;  // transparency

    (void)coordLength;
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


/******************
*  WORLD SYSTEM  *
******************/

World* WorldSystem::addWorld(sol::object t)
{
    worlds.push_back(World(t));
    if (currentWorld == nullptr)
        currentWorld = &(worlds.back());
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

}
