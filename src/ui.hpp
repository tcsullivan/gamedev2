/**
 * @file ui.hpp
 *
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SYSTEM_UI_HPP_
#define SYSTEM_UI_HPP_

#include "events/render.hpp"

#include <entityx/entityx.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <vector>

class UISystem : public entityx::System<UISystem>
{
public:
    void configure(entityx::EntityManager&, entityx::EventManager&) final;
    
    void update(entityx::EntityManager&,
                entityx::EventManager&,
                entityx::TimeDelta) final;

    void createDialogBox(float x, float y, float w, float h);

private:
    struct Box {
        GLuint vbo;
        float x, y, w, h;
    };

    std::vector<Box> m_boxes;

    NewRenderEvent generateDialogBox(Box& box);
};

#endif // SYSTEM_UI_HPP_

