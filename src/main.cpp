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

#include <SDL2/SDL.h>

#include <iostream>
#include <memory>

constexpr const char *title = "gamedev2";
constexpr int width = 640;
constexpr int height = 480;

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
	std::cout << "Hello, world!" << std::endl;

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cerr << "SDL failed to initialize: " << SDL_GetError() <<
			std::endl;
		return -1;
	} else {
		atexit(SDL_Quit);
	}

	std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window
		(SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, width, height, 0), SDL_DestroyWindow);

	if (window.get() == nullptr) {
		std::cerr << "SDL window creation failed: " << SDL_GetError() <<
			std::endl;
	}

	SDL_Delay(1000);

	return 0;
}

