/**
 * @file position.hpp
 * Manages all entity movements
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

#ifndef SYSTEM_PHYSICS_HPP_
#define SYSTEM_PHYSICS_HPP_

#include <entityx/entityx.h>
#include <sol/sol.hpp>

/**
 * @class PhysicsSystem
 * Handles the position and velocity updating of all entities
 */
class PhysicsSystem : public entityx::System<PhysicsSystem>
{
private:
public:
    PhysicsSystem(void) {}

    ~PhysicsSystem(void) {}

    /**
     * Prepares the system for running.
     */
    void configure(entityx::EntityManager& entities,
                   entityx::EventManager& events) final;
    
    /**
     * Updates the position of all entites, and if they have a physics component
     * then we update their velocities accordingly.
     */
    void update(entityx::EntityManager& entites,
                entityx::EventManager& events,
                entityx::TimeDelta dt) final;
};

#endif // SYSTEM_PHYSICS_HPP_

