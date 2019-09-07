/**
 * @file physics.cpp
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

#include "physics.hpp"
#include "components/Physics.hpp"
#include "components/Position.hpp"
#include "components/Velocity.hpp"

void PhysicsSystem::configure([[maybe_unused]]entityx::EntityManager& entities,
                              [[maybe_unused]]entityx::EventManager& events)
{

}
    
void PhysicsSystem::update([[maybe_unused]]entityx::EntityManager& entities,
                           [[maybe_unused]]entityx::EventManager& events,
                           [[maybe_unused]]entityx::TimeDelta dt)
{
    entities.each<Position, Velocity>
        ([dt](entityx::Entity e, Position &pos, Velocity &vel){

        bool has_phys = e.has_component<Physics>();

        pos.x += (vel.x * dt/1000.0);
        pos.y += (vel.y * dt/1000.0);

        double fallPosition = 0;

        // TODO make this intersect world instead of 0 y
        if (has_phys) {
            Physics *p = e.component<Physics>().get();
            // TODO only make this occur when the entity has a hitbox
            if (pos.y == fallPosition) {
                p->standing = true;
                return;
            }

            if (pos.y < fallPosition) {
                pos.y = fallPosition;
                vel.y = 0;
                p->standing = true;
            } else {
                p->standing = false;
                vel.y -= 920 * (dt/1000.0f);
            }
        }
    });
}
