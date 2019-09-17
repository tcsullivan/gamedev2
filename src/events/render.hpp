#ifndef EVENTS_RENDER_HPP_
#define EVENTS_RENDER_HPP_

struct NewRenderEvent
{
    GLuint vbo;
    GLuint tex;
    GLuint normal;
    unsigned int vertex;

    NewRenderEvent(GLuint _vbo, GLuint _tex, GLuint _normal, unsigned int _vertex) :
        vbo(_vbo), tex(_tex), normal(_normal), vertex(_vertex) {}
};

#endif // EVENTS_RENDER_HPP_

