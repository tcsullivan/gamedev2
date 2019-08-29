/**
 * @file window.hpp
 * Manages window creation.
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

#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <entityx/entityx.h>
#include <SDL2/SDL.h>

/**
 * @class WindowSystem
 * Handles the game's window.
 */
class WindowSystem : public entityx::System<WindowSystem> {
private:
	constexpr static const char *title = "gamedev2";
	constexpr static int width = 640;
	constexpr static int height = 480;

	std::unique_ptr<SDL_Window, void (*)(SDL_Window *)> window;
	SDL_GLContext context;

public:
	WindowSystem(void) :
		window(nullptr, SDL_DestroyWindow) {}

	~WindowSystem(void) {
		SDL_GL_DeleteContext(context);
	}

	/**
	 * Creates and initializes the window.
	 * @return Zero on success, non-zero on error
	 */
	int init(void) {
		if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
			std::cerr << "SDL video failed to initialize: "
				<< SDL_GetError() << std::endl;
			return -1;
		}

		// Create window, managed by the unique_ptr
		window.reset(SDL_CreateWindow(title,
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			width, height,
			SDL_WINDOW_OPENGL));

		if (window.get() == nullptr) {
			std::cerr << "SDL window creation failed: "
				<< SDL_GetError() << std::endl;
			return -1;
		}

		context = SDL_GL_CreateContext(window.get());

		return 0;
	}

	/**
	 * Prepares the system for running.
	 */
	void configure([[maybe_unused]] entityx::EntityManager& entities,
		[[maybe_unused]] entityx::EventManager& events) final {
		init();
	}

	/**
	 * Updates/refreshes the window.
	 */
	void update([[maybe_unused]] entityx::EntityManager& entities,
		[[maybe_unused]] entityx::EventManager& events,
		[[maybe_unused]] entityx::TimeDelta dt) final {
		SDL_GL_SwapWindow(window.get());
	}
};

#endif // WINDOW_HPP_

