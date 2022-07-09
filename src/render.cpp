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

#include <config.hpp>
#include <render.hpp>
#include <components/Render.hpp>
#include <components/Position.hpp>
#include <components/Light.hpp>
#include <components/Script.hpp>

void RenderSystem::configure([[maybe_unused]] entityx::EntityManager& entities,
                             [[maybe_unused]] entityx::EventManager& events)
{
    events.subscribe<NewRenderEvent>(*this);
    events.subscribe<WorldMeshUpdateEvent>(*this);
    events.subscribe<entityx::ComponentAddedEvent<Player>>(*this);

    title = Config::get<std::string>("title");
    width = Config::get<int>("screenWidth");
    height = Config::get<int>("screenHeight");

    init();
}

void RenderSystem::update([[maybe_unused]] entityx::EntityManager& entities,
                          [[maybe_unused]] entityx::EventManager& events,
                          [[maybe_unused]] entityx::TimeDelta dt)
{
    // TODO move these to only happen once to speed up rendering
    static GLuint s = worldShader.getProgram();
    static GLuint v = worldShader.getUniform("view");
    static GLuint p = worldShader.getUniform("projection");
    static GLuint m = worldShader.getUniform("model");
    static GLuint a = worldShader.getAttribute("vertex");
    static GLuint t = worldShader.getAttribute("texc");
    static GLuint r = worldShader.getAttribute("trans");

    static GLuint q = worldShader.getUniform("textu");
    static GLuint n = worldShader.getUniform("normu");
    static GLuint b = worldShader.getUniform("AmbientLight");
    static GLuint f = worldShader.getUniform("Flipped");

    static glm::vec3 rot = glm::vec3(0.0f, 0.0f, -1.0f);
    camPos.z = 15.0f;

    /***********
    *  SETUP  *
    ***********/

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_POLYGON_OFFSET_FILL);

    /************
    *  CAMERA  *
    ************/
    try {
        if (player.has_component<Position>()) {
            Position *pos = player.component<Position>().get();
            camPos.y = pos->y;
            camPos.x = pos->x;
        }
    } catch (...) { // If the player doesn't exist or anything goes wrong
        camPos.y = 0.0f;
        camPos.x = 0.0f;
    }

    
    glm::mat4 view = glm::lookAt(camPos,                       // Pos
                                 camPos + rot,                       // Facing
                                 glm::vec3(0.0f, 1.0f, 0.0f)); // Up

    //glm::mat4 projection = glm::perspective(45.0f, 
    //                                        ((float)width/(float)height), 
    //                                        0.01f, 
    //                                        2048.0f);

    //float scale = 40.0f;
    //float scaleWidth = static_cast<float>(width) / scale;
    //float scaleHeight = static_cast<float>(height) / scale;

    //glm::mat4 projection = glm::ortho(-(scaleWidth/2),    // Left
    //                                   (scaleWidth/2),    // Right
    //                                  -(scaleHeight/2),   // Bottom
    //                                   (scaleHeight/2),   // Top
    //                                   100.0f,               // zFar
    //                                  -100.0f                // zNear
    //                                 );

    glm::mat4 projection = glm::perspective(45.0f, 
                                            ((float)width/(float)height), 
                                            0.01f, 
                                            2048.0f);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, -1.0f));

    glUseProgram(s);

    glUniformMatrix4fv(v, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(p, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(m, 1, GL_FALSE, glm::value_ptr(model));

    glEnableVertexAttribArray(a);
    glEnableVertexAttribArray(t);
    glEnableVertexAttribArray(r);

    // Ambient light, for now this is static
    GLfloat amb[4] = {1.0f, 1.0f, 1.0f, 0.0f};
    glUniform4fv(b, 1, amb);

    /**************
    *  LIGHTING  *
    **************/

    std::vector<glm::vec3> lightPos;
    std::vector<glm::vec4> lightColor;
    int lightNum = 0;

    entities.each<Light, Position>([&]
        (entityx::Entity, Light &l, Position &p){

        lightPos.push_back(glm::vec3(p.x, p.y, 1.0));
        lightColor.push_back(glm::vec4(l.r, l.g, l.b, l.strength));
        lightNum++;
    });

    glUniform1i(worldShader.getUniform("LightNum"), lightNum);
    glUniform3fv(worldShader.getUniform("LightPos"), 
                 lightPos.size(),
                 reinterpret_cast<GLfloat*>(lightPos.data()));
    glUniform4fv(worldShader.getUniform("LightColor"),
                 lightColor.size(),
                 reinterpret_cast<GLfloat*>(lightColor.data()));

    /*************
    *  DRAWING  *
    *************/

    entities.each<Render, Position>(
        [this](entityx::Entity, Render &rend, Position &p) {

        if (!rend.visible)
            return;

        // If our component was created via script, call the entity's
        //  RenderIdle function
        //if (e.has_component<Scripted>()) {
        //    e.component<Scripted>()->updateRender();
        //}
        
        auto& c = rend.corners;

        GLuint tri_vbo;
        GLfloat tri_data[] = {
                p.x+c[0].x, p.y+c[0].y, 0.0f, 0.0f, 1.0f, 1.0f,
                p.x+c[1].x, p.y+c[1].y, 0.0f, 1.0f, 1.0f, 1.0f,
                p.x+c[2].x, p.y+c[2].y, 0.0f, 0.0f, 0.0f, 1.0f,
                                   
                p.x+c[1].x, p.y+c[1].y, 0.0f, 1.0f, 1.0f, 1.0f,
                p.x+c[3].x, p.y+c[3].y, 0.0f, 1.0f, 0.0f, 1.0f,
                p.x+c[2].x, p.y+c[2].y, 0.0f, 0.0f, 0.0f, 1.0f,
        };

        bool flipped = false;

        // TODO flip nicely (aka model transformations)
        if (rend.flipX) {
            std::swap(tri_data[3], tri_data[9]);
            tri_data[15] = tri_data[3];

            std::swap(tri_data[27], tri_data[33]);
            tri_data[21] = tri_data[27];

            flipped = true;
        }

        glUniform1i(f, flipped ? 1 : 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, rend.texture.tex);
        glUniform1i(q, 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, rend.normal.tex);
        glUniform1i(n, 1);

        glGenBuffers(1, &tri_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, tri_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(tri_data), tri_data, GL_STREAM_DRAW);

        glVertexAttribPointer(a, 3, GL_FLOAT, GL_FALSE,
                              6*sizeof(float), 0);
        glVertexAttribPointer(t, 2, GL_FLOAT, GL_FALSE,
                              6*sizeof(float), (void*)(3*sizeof(float)));
        glVertexAttribPointer(r, 1, GL_FLOAT, GL_FALSE,
                              6*sizeof(float), (void*)(5*sizeof(float)));
        glDrawArrays(GL_TRIANGLES, 0, 6);
    });
    glUniform1i(f, 0);

    for (auto& w : worldRenders) {
        auto& layer = w.second;

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, layer.tex);
        glUniform1i(q, 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, layer.normal);
        glUniform1i(n, 1);

        glBindBuffer(GL_ARRAY_BUFFER, w.first);
        glVertexAttribPointer(a, 3, GL_FLOAT, GL_FALSE,
                              6*sizeof(float), 0);
        glVertexAttribPointer(t, 2, GL_FLOAT, GL_FALSE, 
                              6*sizeof(float), (void*)(3*sizeof(float)));
        glVertexAttribPointer(r, 1, GL_FLOAT, GL_FALSE, 
                              6*sizeof(float), (void*)(5*sizeof(float)));
        glDrawArrays(GL_TRIANGLES, 0, layer.vertex);
    }

    glDisableVertexAttribArray(a);
    glDisableVertexAttribArray(t);

    glUseProgram(0);

    /******************
    *  UI RENDERING  *
    ******************/

    static GLuint uiS   = uiShader.getProgram();
    static GLuint uiS_v = uiShader.getUniform("view");
    static GLuint uiS_p = uiShader.getUniform("projection");
    static GLuint uiS_m = uiShader.getUniform("model");
    static GLuint uiS_a = uiShader.getAttribute("coord2d");
    static GLuint uiS_t = uiShader.getAttribute("tex_coord");
    static GLuint uiS_q = uiShader.getUniform("sampler");

    glUseProgram(uiS);

    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f),  // Pos
                       glm::vec3(0.0f, 0.0f, 1.0f),  // Facing
                       glm::vec3(0.0f, 1.0f, 0.0f)); // Up

    projection = glm::ortho(0.0f,  // Left
                            static_cast<float>(width),  // Right
                            -static_cast<float>(height), // Top
                            0.0f,
                             100.0f,      // zFar
                            -100.0f);     // zNear

    model = glm::mat4(1.0f);

    glUniformMatrix4fv(uiS_v, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(uiS_p, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(uiS_m, 1, GL_FALSE, glm::value_ptr(model));

    glEnableVertexAttribArray(uiS_a);
    glEnableVertexAttribArray(uiS_t);

    // Update all UI VBOs
    for (auto& r : uiRenders) {
        auto& render = r.second;

        glActiveTexture(GL_TEXTURE9);
        glBindTexture(GL_TEXTURE_2D, render.tex);
        glUniform1i(uiS_q, 9);

        glBindBuffer(GL_ARRAY_BUFFER, r.first);
        glVertexAttribPointer(uiS_a, 3, GL_FLOAT, GL_FALSE,
                              6*sizeof(float), 0);
        glVertexAttribPointer(uiS_t, 2, GL_FLOAT, GL_FALSE, 
                              6*sizeof(float), (void*)(3*sizeof(float)));
        glDrawArrays(GL_TRIANGLES, 0, render.vertex);
    }

    glDisableVertexAttribArray(uiS_a);
    glDisableVertexAttribArray(uiS_t);

    /*************
    *  CLEANUP  *
    *************/

    glUseProgram(0);

    glDisable(GL_POLYGON_OFFSET_FILL);
    glDisable(GL_CULL_FACE);

    SDL_GL_SwapWindow(window.get());
}

int RenderSystem::init(void)
{

    if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL video failed to initialize: "
            << SDL_GetError() << std::endl;
        return -1;
    }

    // Create window, managed by the unique_ptr
    window.reset(SDL_CreateWindow(title.c_str(),
                 SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                 width, height,
                 SDL_WINDOW_OPENGL));

    if (window.get() == nullptr) {
        std::cerr << "SDL window creation failed: "
            << SDL_GetError() << std::endl;
        return -1;
    }

    // Select an OpenGL 4.3 profile.
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    context = SDL_GL_CreateContext(window.get());

    glewExperimental = GL_TRUE;
    if (auto err = glewInit(); err != GLEW_OK){
        std::cerr << "GLEW was not able to initialize! Error: " << 
            glewGetErrorString(err) << std::endl;
        return -1;
    }

    SDL_GL_SetSwapInterval(1);

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

    worldShader.createProgram("Shaders/world.vert", "Shaders/world.frag");

    worldShader.addUniform("projection");
    worldShader.addUniform("view");
    worldShader.addUniform("model");

    worldShader.addAttribute("vertex");
    worldShader.addAttribute("texc");
    worldShader.addAttribute("trans");

    worldShader.addUniform("textu");
    worldShader.addUniform("normu");

    worldShader.addUniform("LightPos");
    worldShader.addUniform("LightColor");
    worldShader.addUniform("LightNum");
    worldShader.addUniform("AmbientLight");
    worldShader.addUniform("Flipped");

    uiShader.createProgram("Shaders/ui.vert", "Shaders/ui.frag");

    uiShader.addUniform("projection");
    uiShader.addUniform("view");
    uiShader.addUniform("model");

    uiShader.addAttribute("coord2d");
    uiShader.addAttribute("tex_coord");

    uiShader.addUniform("sampler");

    glEnableVertexAttribArray(worldShader.getAttribute("vertex"));
    glEnableVertexAttribArray(uiShader.getAttribute("coord2d"));

    //glPolygonOffset(1.0, 1.0);

    //glClearColor(0.6, 0.8, 1.0, 0.0);
    
    camPos = glm::vec3(0.0f, 0.0f, 5.0f);

    return 0;
}

/************
*  EVENTS  *
************/

void RenderSystem::receive(const NewRenderEvent &nre)
{
    uiRenders.insert_or_assign(nre.vbo,
                               UIRenderData(nre.tex, nre.normal, nre.vertex));
}

void RenderSystem::receive(const WorldMeshUpdateEvent &wmu)
{
    worldRenders.insert_or_assign(
        wmu.worldVBO,
        WorldRenderData(wmu.worldTexture, 
                        wmu.worldNormal,
                        wmu.numVertex)
    );
}

void RenderSystem::receive(const entityx::ComponentAddedEvent<Player> &cae)
{
    player = cae.entity;
}

