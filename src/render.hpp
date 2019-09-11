/**
 * @file render.hpp
 * Handles all in game rendering
 *
 * Copyright (C) 2019 Clyne Sullivan
 * Copyright (C) 2019  Belle-Isle, Andrew <drumsetmonkey@gmail.com>
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

#ifndef SYSTEM_RENDER_HPP_
#define SYSTEM_RENDER_HPP_

#include <entityx/entityx.h>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/noise.hpp>

#include "shader.hpp"

class RenderSystem : public entityx::System<RenderSystem>
{
private:
    constexpr static const char *title = "gamedev2";
    constexpr static int width = 1280;
    constexpr static int height = 720;

    std::unique_ptr<SDL_Window, void (*)(SDL_Window *)> window;
    SDL_GLContext context;

    Shader worldShader;
    glm::vec3 camPos;

public:
    RenderSystem(void) :
        window(nullptr, SDL_DestroyWindow) {}

    ~RenderSystem(void)
    {
        SDL_GL_DeleteContext(context);
        SDL_Quit();
    }

    /**
     * Prepares the system for running.
     */
    void configure(entityx::EntityManager& entities,
                   entityx::EventManager& events) final;
    
    /**
     * Updates the render system.
     */
    void update(entityx::EntityManager& entities,
                entityx::EventManager& events,
                entityx::TimeDelta dt) final;

    /**
     * Initializes the rendering system
     * @return Zero on success, non-zero on error
     */
    int init(void);
};

#endif // SYSTEM_RENDER_HPP_

