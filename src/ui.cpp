#include "ui.hpp"

#include "events/render.hpp"
#include "text.hpp"

#include <SDL2/SDL_opengl.h>
#include <iostream>
#include <string>

static NewRenderEvent NRE (0, 0, 0, 0);
static const unsigned int NRE_TEX_DATA = 0xBBBBBBBB;
static std::basic_string<TextMeshData> buffer;

void UISystem::configure(entityx::EntityManager&, entityx::EventManager&)
{
}

void UISystem::update(entityx::EntityManager&,
    entityx::EventManager& events,
    entityx::TimeDelta)
{
    static bool t = false;

    if (!t) {
        t = true;

        glGenBuffers(1, &NRE.vbo);
        glGenTextures(1, &NRE.tex);

        glBindTexture(GL_TEXTURE_2D, NRE.tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &NRE_TEX_DATA);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        buffer += { 30, -50, -5, 0, 0, 1 };
        buffer += { 160, -50, -5, 0, 0, 1 };
        buffer += { 160, -10, -5, 0, 0, 1 };
        buffer += { 160, -10, -5, 0, 0, 1 };
        buffer += { 30, -10, -5, 0, 0, 1 };
        buffer += { 30, -50, -5, 0, 0, 1 };

        glBindBuffer(GL_ARRAY_BUFFER, NRE.vbo);
        glBufferData(GL_ARRAY_BUFFER,
                     buffer.size() * sizeof(TextMeshData), buffer.data(),
                     GL_DYNAMIC_DRAW);

        NRE.normal = 0;
        NRE.vertex = buffer.size();
        events.emit<NewRenderEvent>(NRE);
    }
}

