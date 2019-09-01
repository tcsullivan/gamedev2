#version 430

#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif

uniform sampler2D textu;

in vec2 texCoord;
out vec4 FragColor;

void main()
{
    FragColor = texture(textu, texCoord);
}
