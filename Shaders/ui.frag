#version 130

uniform sampler2D sampler;

in vec2 texCoord;
in vec4 color;

out vec4 FragColor;

void main(){
    vec4 pixelColor = texture2D(sampler, vec2(texCoord.x, texCoord.y));
    //TODO allow antialiasing
	//if (pixelColor.w != 1.0f)
	//	discard;
	FragColor = pixelColor * color;
}
