/**
 * @file player.cpp
 * Manages player input.
 *
 * Copyright (C) 2019 Clyne Sullivan
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

#include "player.hpp"

#include "components/Velocity.hpp"

void PlayerSystem::configure([[maybe_unused]] entityx::EntityManager& entities,
                             entityx::EventManager& events)
{
    events.subscribe<entityx::ComponentAddedEvent<Player>>(*this);
    events.subscribe<entityx::ComponentRemovedEvent<Player>>(*this);
    events.subscribe<KeyUpEvent>(*this);
    events.subscribe<KeyDownEvent>(*this);
}

void PlayerSystem::update([[maybe_unused]] entityx::EntityManager& entites,
                          [[maybe_unused]] entityx::EventManager& events,
                          [[maybe_unused]] entityx::TimeDelta dt)
{
}

void PlayerSystem::receive(const entityx::ComponentAddedEvent<Player>& cae)
{
    player = cae.entity;
}

void PlayerSystem::receive(const entityx::ComponentRemovedEvent<Player>& cre)
{
    if (player == cre.entity)
        player.invalidate();
}

void PlayerSystem::receive(const KeyDownEvent& kue)
{
    if (player.valid()) {
        if (kue.sym == SDLK_a) {
            if (auto vel = player.component<Velocity>(); vel)
                vel->x -= GROUND_VELOCITY;
        } else if (kue.sym == SDLK_d) {
            if (auto vel = player.component<Velocity>(); vel)
                vel->x += GROUND_VELOCITY;
        }
    }
}

void PlayerSystem::receive(const KeyUpEvent& kue)
{
    if (player.valid()) {
        if (kue.sym == SDLK_a) {
            if (auto vel = player.component<Velocity>(); vel)
                vel->x += GROUND_VELOCITY;
        } else if (kue.sym == SDLK_d) {
            if (auto vel = player.component<Velocity>(); vel)
                vel->x -= GROUND_VELOCITY;
        }
    }
}

