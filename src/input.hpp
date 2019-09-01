/**
 * @file window.hpp
 * Handles user input received from SDL.
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

#ifndef SYSTEM_INPUT_HPP_
#define SYSTEM_INPUT_HPP_

#include <entityx/entityx.h>
#include <SDL2/SDL.h>

/**
 * @class KeyUpEvent
 * Stores info regarding key releases.
 */
struct KeyUpEvent
{
    SDL_Keycode sym;
    Uint16 mod;

    KeyUpEvent(const SDL_Keysym& keysym) :
        sym(keysym.sym), mod(keysym.mod) {}
};

/**
 * @class KeyDownEvent
 * Stores info regarding key presses.
 */
struct KeyDownEvent {
    SDL_Keycode sym;
    Uint16 mod;

    KeyDownEvent(const SDL_Keysym& keysym) :
        sym(keysym.sym), mod(keysym.mod) {}
};

/**
 * @class InputSystem
 * Listens for user input from SDL, and emits input events accordingly.
 */
class InputSystem : public entityx::System<InputSystem>
{
public:
    /**
     * Prepares the system for running.
     */
    void configure([[maybe_unused]] entityx::EntityManager& entities,
                   [[maybe_unused]] entityx::EventManager& events) final {}

    /**
     * Updates the system by checking for SDL events.
     */
    void update([[maybe_unused]] entityx::EntityManager& entities,
                [[maybe_unused]] entityx::EventManager& events,
                [[maybe_unused]] entityx::TimeDelta dt) final
    {
        for (SDL_Event event; SDL_PollEvent(&event);) {
            switch (event.type) {
            case SDL_KEYUP:
                if (auto key = event.key; key.repeat == 0)
                    events.emit<KeyUpEvent>(key.keysym);
                break;
            case SDL_KEYDOWN:
                if (auto key = event.key; key.repeat == 0)
                    events.emit<KeyDownEvent>(key.keysym);
                break;
            default:
                break;
            }
        }
    }
};

#endif // SYSTEM_INPUT_HPP_

