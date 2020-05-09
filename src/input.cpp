/**
 * @file input.cpp
 * Handles user input received from SDL.
 *
 * Copyright (C) 2020 Clyne Sullivan
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

#include "input.hpp"

#include "components/EventListener.hpp"
#include "components/Script.hpp"

InputSystem::InputSystem() :
    isMouseDown(false) {}

/**
 * Prepares the system for running.
 */
void InputSystem::configure([[maybe_unused]] entityx::EntityManager& entities,
                            [[maybe_unused]] entityx::EventManager& events) {}

/**
 * Updates the system by checking for SDL events.
 */
void InputSystem::update(entityx::EntityManager& entities,
            entityx::EventManager& events,
            [[maybe_unused]] entityx::TimeDelta dt)
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
        case SDL_MOUSEBUTTONDOWN:
            if (!isMouseDown) {
                isMouseDown = true;
                entities.each<EventListener>(
                    [&event](entityx::Entity e, EventListener& el) {
                        el.tryListener("MousePressed",
                            e.component<Scripted>()->caller,
                            event.button.x,
                            event.button.y,
                            event.button.button);
                    });
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (isMouseDown) {
                isMouseDown = false;
                entities.each<EventListener>(
                    [&event](entityx::Entity e, EventListener& el) {
                        el.tryListener("MouseReleased",
                            e.component<Scripted>()->caller,
                            event.button.x,
                            event.button.y,
                            event.button.button);
                    });
            }
            break;
        default:
            break;
        }
    }
}

