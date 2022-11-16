/**
 * @file ui.cpp
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

#include "ui.hpp"

#include "text.hpp"

#include <SDL2/SDL_opengl.h>
#include <iostream>
#include <string>

static const unsigned int NRE_TEX_DATA = 0xBBBBBBBB;

void UISystem::configure(entityx::EntityManager&, entityx::EventManager &events)
{
    events.subscribe<HideDialog>(*this);
}

void UISystem::createDialogBox(float x, float y, float w, float h)
{
    // Queue for generation in the main thread.
    // TODO Should y inversion be hidden in the render system?
    m_boxes.emplace_back(0, x, -y - h, w, h);
}

void UISystem::update(entityx::EntityManager&,
    entityx::EventManager& events,
    entityx::TimeDelta)
{
    if (m_clear_boxes) {
        m_clear_boxes = false;

        for (const auto& b : m_boxes) {
            if (b.vbo != 0) {
                events.emit<DelRenderEvent>(b.vbo);
            }
        }

        m_boxes.clear();
    } else {
        for (auto& b : m_boxes) {
            if (b.vbo == 0) {
                auto nre = generateDialogBox(b);
                events.emit<NewRenderEvent>(nre);
            }
        }
    }
}

void UISystem::receive(const HideDialog&)
{
    m_clear_boxes = true;
}

NewRenderEvent UISystem::generateDialogBox(Box& box)
{
    NewRenderEvent nre;
    std::basic_string<TextMeshData> buffer;

    glGenBuffers(1, &nre.vbo);
    glGenTextures(1, &nre.tex);
    box.vbo = nre.vbo;

    glBindTexture(GL_TEXTURE_2D, nre.tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, &NRE_TEX_DATA);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    buffer += { box.x, box.y, -5, 0, 0, 1 };
    buffer += { box.x + box.w, box.y, -5, 0, 0, 1 };
    buffer += { box.x + box.w, box.y + box.h, -5, 0, 0, 1 };
    buffer += { box.x + box.w, box.y + box.h, -5, 0, 0, 1 };
    buffer += { box.x, box.y + box.h, -5, 0, 0, 1 };
    buffer += { box.x, box.y, -5, 0, 0, 1 };

    glBindBuffer(GL_ARRAY_BUFFER, nre.vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 buffer.size() * sizeof(TextMeshData), buffer.data(),
                 GL_DYNAMIC_DRAW);

    nre.normal = 0;
    nre.vertex = buffer.size();
    return nre;
}

