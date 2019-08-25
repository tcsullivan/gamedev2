/**
 * @file main.cpp
 * The main file, where the magic begins.
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

#include <entityx.h>

#include <SDL2/SDL.h>

#include <atomic>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

constexpr const char *title = "gamedev2";
constexpr int width = 640;
constexpr int height = 480;

std::atomic_bool shouldRun;

static void renderLoop(void);
static void logicLoop(void);

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cerr << "SDL failed to initialize: " << SDL_GetError()
			<< std::endl;
		return -1;
	} else {
		atexit(SDL_Quit);
	}

	// Create our window
	std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window
		(SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, width, height, 0), SDL_DestroyWindow);

	if (window.get() == nullptr) {
		std::cerr << "SDL window creation failed: " << SDL_GetError()
			<< std::endl;
		return -1;
	}

	// Start game
	shouldRun.store(true);
	std::thread logic (logicLoop);
	renderLoop();
	logic.join();

	return 0;
}

void renderLoop(void)
{
	using namespace std::chrono_literals;

	// TODO render
	while (shouldRun.load())
		std::this_thread::sleep_for(100ms);
}

void logicLoop(void)
{
	using namespace std::chrono_literals;

	std::cout << "Press escape to exit." << std::endl;

	while (shouldRun.load()) {
		for (SDL_Event event; SDL_PollEvent(&event);) {
			switch (event.type) {
			case SDL_KEYUP:
				// Exit game on escape
				if (event.key.keysym.sym == SDLK_ESCAPE)
					shouldRun.store(false);
				break;
			default:
				break;
			}
		}

		std::this_thread::sleep_for(100ms);
	}
}

