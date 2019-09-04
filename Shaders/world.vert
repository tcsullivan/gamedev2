#version 130

//layout(location = 0)in vec3 vertex;
in vec3 vertex;
in vec2 texc;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 texCoord;
out vec4 fragCoord;

void main()
{
    texCoord = texc;
    fragCoord = vec4(vertex, 1.0f);
    gl_Position = projection * view * model * fragCoord;
}
