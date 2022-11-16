/**
 * @file player.cpp
 * Manages player input.
 *
 * Copyright (C) 2022 Clyne Sullivan
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

#include "components/EventListener.hpp"
#include "components/Script.hpp"
#include "components/Velocity.hpp"

void PlayerSystem::configure(entityx::EntityManager&,
                             entityx::EventManager& events)
{
    events.subscribe<entityx::ComponentAddedEvent<Player>>(*this);
    events.subscribe<entityx::ComponentRemovedEvent<Player>>(*this);
}

void PlayerSystem::update(entityx::EntityManager&,
                          entityx::EventManager&,
                          entityx::TimeDelta)
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

