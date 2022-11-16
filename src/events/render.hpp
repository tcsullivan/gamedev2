/*
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

#ifndef EVENTS_RENDER_HPP_
#define EVENTS_RENDER_HPP_

#include <GL/glew.h>

struct NewRenderEvent
{
    GLuint vbo;
    GLuint tex;
    GLuint normal;
    unsigned int vertex;

    NewRenderEvent() = default;
    NewRenderEvent(GLuint _vbo, GLuint _tex, 
                   GLuint _normal, unsigned int _vertex) :
        vbo(_vbo), tex(_tex), normal(_normal), vertex(_vertex) {}
};

struct DelRenderEvent
{
    GLuint vbo;

    DelRenderEvent(GLuint _vbo) :
        vbo(_vbo) {}
};

struct WorldMeshUpdateEvent
{
    GLuint worldVBO;
    GLuint worldTexture;
    GLuint worldNormal;
    unsigned int numVertex;

    WorldMeshUpdateEvent(GLuint v, GLuint t, 
                         GLuint n, unsigned int p) :
        worldVBO(v), worldTexture(t), worldNormal(n), numVertex(p) {}

    WorldMeshUpdateEvent() {};
};

#endif // EVENTS_RENDER_HPP_

