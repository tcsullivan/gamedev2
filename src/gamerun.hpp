/**
 * @file window.hpp
 * Handles key press for exiting the game.
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef GAMERUN_HPP_
#define GAMERUN_HPP_

#include "input.hpp"

#include <entityx/entityx.h>

#include <atomic>
#include <iostream>

/**
 * @class GameRunSystem
 * Listens for a key event to tell the game to exit.
 */
class GameRunSystem : public entityx::System<GameRunSystem>,
	public entityx::Receiver<GameRunSystem> {
private:
	std::atomic_bool shouldRunFlag;

public:
	/**
	 * Configures the system to wait for the exit event.
	 */
	void configure([[maybe_unused]] entityx::EntityManager& entities,
		entityx::EventManager& events) {
		shouldRunFlag.store(true);

		events.subscribe<KeyUpEvent>(*this);

		std::cout << "Press escape to exit." << std::endl;
	}

	// Unused
	void update([[maybe_unused]] entityx::EntityManager& entities,
		[[maybe_unused]] entityx::EventManager& events,
		[[maybe_unused]] entityx::TimeDelta dt) final {}

	/**
	 * Receiver for key release events, used to listen for game exit key.
	 */
	void receive(const KeyUpEvent& kue) {
		if (kue.sym == SDLK_ESCAPE)
			shouldRunFlag.store(false);
	}

	/**
	 * Checks if the game exit key has been pressed.
	 * @return True if the game should exit
	 */
	inline bool shouldRun(void) const {
		return shouldRunFlag.load();
	}
};

#endif // GAMERUN_HPP_

