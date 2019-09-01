/**
 * @file player.hpp
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

#ifndef SYSTEM_PLAYER_HPP_
#define SYSTEM_PLAYER_HPP_

#include <SDL2/SDL.h>
#include <entityx/entityx.h>

#include "components/Player.hpp"
#include "input.hpp"

/**
 * @class PlayerSystem
 * Controls player entity movement.
 */
class PlayerSystem : public entityx::System<PlayerSystem>,
                     public entityx::Receiver<PlayerSystem>
{
private:
    /**
     * Defines player's horizontal movement velocity.
     */
    constexpr static double GROUND_VELOCITY = 100;

    entityx::Entity player;

public:
    /**
     * Prepares the system for running.
     */
    void configure(entityx::EntityManager& entities,
                   entityx::EventManager& events) final;

    /**
     * Updates the scripting system.
     */
    void update(entityx::EntityManager& entites,
                entityx::EventManager& events,
                entityx::TimeDelta dt) final;

    /**
     * Captures the player entity.
     */
    void receive(const entityx::ComponentAddedEvent<Player>& cae);

    /**
     * Invalidates the system's player entity (assume player is gone).
     */
    void receive(const entityx::ComponentRemovedEvent<Player>& cre);

    /**
     * Applies velocity based on key press.
     */
    void receive(const KeyDownEvent& kue);

    /**
     * Removes applied velocity
     */
    void receive(const KeyUpEvent& kue);
};

#endif // SYSTEM_PLAYER_HPP_

