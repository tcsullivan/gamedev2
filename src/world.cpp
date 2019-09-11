/**
 * @file world.cpp
 *
 * Copyright (C) 2019  Belle-Isle, Andrew <drumsetmonkey@gmail.com>
 * Author: Belle-Isle, Andrew <drumsetmonkey@gmail.com>
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
World::World(sol::object ref)
{
    if (ref.get_type() == sol::type::table) {

    } else {
        // TODO better logging
        std::cerr << "World paramaters must be stored in a table" << std::endl;
    }
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
