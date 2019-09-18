#version 130

in vec3 coord2d;
in vec2 tex_coord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 texCoord;
out vec4 color;

void main(){
    texCoord = tex_coord;
    color = vec4(1.0, 1.0, 1.0, 1.0);
    //color = tex_color;
    //gl_Position = ortho * vec4(coord2d.xyz, 1.0);
    gl_Position = projection * view * model * vec4(coord2d, 1.0f);
}
