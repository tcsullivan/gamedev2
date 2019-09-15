/*
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

#ifndef EVENTS_WORLD_HPP_
#define EVENTS_WORLD_HPP_

#include "world.hpp"

struct WorldChangeEvent
{
    World* newWorld;

    WorldChangeEvent(World* w) :
        newWorld(w) {}
};

struct WorldMeshUpdateEvent
{
    GLuint worldVBO;

    WorldMeshUpdateEvent(GLuint v) :
        worldVBO(v) {}
};

#endif//EVENTS_WORLD_HPP
