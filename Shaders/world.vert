#version 130

//layout(location = 0)in vec3 vertex;
in vec3 vertex;
in vec2 texc;
in float trans;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out float fragTrans;
out vec2 texCoord;
out vec4 fragCoord;

void main()
{
    fragTrans = trans;
    texCoord = texc;
    fragCoord = vec4(vertex, 1.0f);
    gl_Position = projection * view * model * fragCoord;
}
