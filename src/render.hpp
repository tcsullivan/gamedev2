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
#include "world.hpp"
#include "components/Player.hpp"

#include "events/render.hpp"

#include <map>

struct UIRenderData
{
    GLuint tex;
    GLuint normal;
    unsigned int vertex;

    UIRenderData(GLuint _tex, GLuint _normal, unsigned int _vertex) :
        tex(_tex), normal(_normal), vertex(_vertex) {}
};

struct WorldRenderData
{
    GLuint tex;
    GLuint normal;
    unsigned int vertex;

    WorldRenderData(GLuint _tex, GLuint _normal, unsigned int _vertex) :
        tex(_tex), normal(_normal), vertex(_vertex) {}
};

class RenderSystem : public entityx::System<RenderSystem>,
                     public entityx::Receiver<RenderSystem>
{
private:
    std::string title;
    int width;
    int height;

    std::unique_ptr<SDL_Window, void (*)(SDL_Window *)> window;
    SDL_GLContext context;

    Shader worldShader;
    Shader uiShader;
    glm::vec3 camPos;

    // Map of VBOs and their render data
    std::map<GLuint, UIRenderData> uiRenders;
    std::map<GLuint, WorldRenderData> worldRenders;

    entityx::Entity player; // Save the player so we can track the camera

public:
    RenderSystem() :
        window(nullptr, SDL_DestroyWindow) {}

    ~RenderSystem()
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
    int init();

    glm::vec3 getCameraPosition() const {
        return camPos;
    }

    Position uiToWorldCoord(float x, float y) const;

    /**
     * Returns the width of the camera's view in world coordinates.
     */
    float getWorldViewWidth() const;

    /**
     * Returns the height of the camera's view in world coordinates.
     */
    float getWorldViewHeight() const;


    /************
    *  EVENTS  *
    ************/
    void receive(const WorldMeshUpdateEvent &wmu);
    void receive(const NewRenderEvent &nre);
    void receive(const DelRenderEvent &dre);
    void receive(const entityx::ComponentAddedEvent<Player> &cae);
};

#endif // SYSTEM_RENDER_HPP_

