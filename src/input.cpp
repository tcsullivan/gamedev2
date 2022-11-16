/**
 * @file input.cpp
 * Handles user input received from SDL.
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
                            [[maybe_unused]] entityx::EventManager& events)
{
    bindings.emplace(Binding {SDLK_a, 0}, "MoveLeft");
    bindings.emplace(Binding {SDLK_d, 0}, "MoveRight");
    bindings.emplace(Binding {SDLK_SPACE, 0}, "JumpKey");
}

/**
 * Updates the system by checking for SDL events.
 */
void InputSystem::update(entityx::EntityManager& entities,
            entityx::EventManager& events,
            entityx::TimeDelta)
{
    std::string listener;

    for (SDL_Event event; SDL_PollEvent(&event);) {
        std::function tryListener =
            [&listener](entityx::Entity, EventListener& el, Scripted& s) {
                el.tryListener(listener, s.caller);
            };

        if (event.type == SDL_KEYUP) {
            if (event.key.repeat == 0) {
                auto b = bindings.find({event.key.keysym.sym, event.key.keysym.mod});

                if (b != bindings.end())
                    listener = b->second + "Released";
                else
                    events.emit<KeyUpEvent>(event.key.keysym);
            }
        } else if (event.type == SDL_KEYDOWN) {
            if (event.key.repeat == 0) {
                auto b = bindings.find({event.key.keysym.sym, event.key.keysym.mod});

                if (b != bindings.end())
                    listener = b->second + "Pressed";
                else
                    events.emit<KeyDownEvent>(event.key.keysym);
            }
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (!isMouseDown) {
                isMouseDown = true;
                listener = "MousePressed";

                tryListener =
                    [&listener, m = event.button]
                    (entityx::Entity, EventListener& el, Scripted& s) {
                        el.tryListener(listener, s.caller,
                                       m.x, m.y, m.button);
                    };
            }
        } else if (event.type == SDL_MOUSEBUTTONUP) {
            if (isMouseDown) {
                isMouseDown = false;
                listener = "MouseReleased";

                tryListener =
                    [&listener, m = event.button]
                    (entityx::Entity, EventListener& el, Scripted& s) {
                        el.tryListener(listener, s.caller,
                                       m.x, m.y, m.button);
                    };
            }
        }

        if (!listener.empty()) {
            entities.each<EventListener, Scripted>(tryListener);
            listener.clear();
        }
    }
}

