/**
 * @file render.cpp
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

#include <render.hpp>
#include <components/Render.hpp>
#include <components/Position.hpp>

void RenderSystem::configure([[maybe_unused]]entityx::EntityManager& entities,
                             [[maybe_unused]]entityx::EventManager& events)
{
    init();
}

void RenderSystem::update([[maybe_unused]] entityx::EntityManager& entities,
                          [[maybe_unused]] entityx::EventManager& events,
                          [[maybe_unused]] entityx::TimeDelta dt)
{
    GLuint s = worldShader.getProgram();
    GLuint v = worldShader.getUniform("view");
    GLuint p = worldShader.getUniform("projection");
    GLuint m = worldShader.getUniform("model");
    GLuint a = worldShader.getAttribute("vertex");

    /***********
    *  SETUP  *
    ***********/
    
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f),  // Pos
                                 glm::vec3(0.0f, 0.0f, 0.0f),  // Facing
                                 glm::vec3(0.0f, 1.0f, 0.0f)); // Up

    //glm::mat4 projection = glm::perspective(45.0f, 
    //                                        ((float)width/(float)height), 
    //                                        0.01f, 
    //                                        2048.0f);

    glm::mat4 projection = glm::ortho(-((float)width/2),    // Left
                                       ((float)width/2),    // Right
                                      -((float)height/2),   // Bottom
                                       ((float)height/2),   // Top
                                      -10.0f,               // zNear
                                       10.0f                // zFar
                                     );

    glm::mat4 model = glm::mat4(1.0f);

    glUseProgram(s);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glUniformMatrix4fv(v, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(p, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(m, 1, GL_FALSE, glm::value_ptr(model));

    glEnable(GL_CULL_FACE);
    glEnable(GL_POLYGON_OFFSET_FILL);

    glEnableVertexAttribArray(a);

    /*************
    *  DRAWING  *
    *************/

    entities.each<Render, Position>(
            [this, a](entityx::Entity, Render &r, Position &p){

        if (!r.visible)
            return;

        GLuint tri_vbo;
        GLfloat tri_data[] = {
                (float)p.x-10.0f, (float)p.y-10.0f, 00.0f,
                (float)p.x+10.0f, (float)p.y-10.0f, 00.0f,
                (float)p.x+00.0f, (float)p.y+10.0f, 00.0f,
        };

        glGenBuffers(1, &tri_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, tri_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(tri_data), tri_data, GL_STREAM_DRAW);

        glVertexAttribPointer(a, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    
    });
    

    /*************
    *  CLEANUP  *
    *************/
    glDisableVertexAttribArray(a);

    glDisable(GL_POLYGON_OFFSET_FILL);
    glDisable(GL_CULL_FACE);

    glUseProgram(0);

    SDL_GL_SwapWindow(window.get());
}

int RenderSystem::init(void)
{
    // Select an OpenGL 4.3 profile.
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

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

    GLenum err;
    glewExperimental = GL_TRUE;
    if((err=glewInit()) != GLEW_OK){
        std::cerr << "GLEW was not able to initialize! Error: " << 
            glewGetErrorString(err) << std::endl;
        return -1;
    }

    SDL_GL_SetSwapInterval(0);

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

    worldShader.createProgram("Shaders/world.vert", "Shaders/world.frag");

    worldShader.addUniform("projection");
    worldShader.addUniform("view");
    worldShader.addUniform("model");

    worldShader.addAttribute("vertex");

    glEnableVertexAttribArray(worldShader.getAttribute("vertex"));
    glUseProgram(worldShader.getProgram());

    // TODO
    //glPolygonOffset(1.0, 1.0);

    glClearColor(0.6, 0.8, 1.0, 0.0);

    return 0;
}
