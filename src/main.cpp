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

#define SOL_ALL_SAFETIES_ON = 1

#include "engine.hpp"

#include <SDL2/SDL.h>

#include <iostream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    // Initialize SDL
    if (SDL_Init(0) != 0) {
        std::cerr << "SDL failed to initialize: " << SDL_GetError()
            << std::endl;
        return -1;
    } else {
        atexit(SDL_Quit);
    }

    // Create the engine
    Engine engine;
    engine.init();

    // Go go go!
    engine.run();

    return 0;
}

