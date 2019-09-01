#version 430

//layout(location = 0)in vec3 vertex;
in vec3 vertex;
in vec2 texc;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 texCoord;

void main()
{
    texCoord = texc;
    gl_Position = projection * view * model * vec4(vertex, 1.0f);
}
