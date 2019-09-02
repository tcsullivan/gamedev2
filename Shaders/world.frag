#version 430

#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif

uniform sampler2D textu;
uniform sampler2D normu;

in vec2 texCoord;
in vec4 fragCoord;
out vec4 FragColor;

uniform vec3 LightPos[32];
uniform vec4 LightColor[32];
uniform int LightNum;

void main()
{
    vec3 Falloff = vec3(0.1, 0.2, 0.0);

    vec4 DiffuseColor = texture2D(textu, texCoord);

    if (DiffuseColor.a < 0.1f)
        discard;

    vec3 NormalMap = texture2D(normu, texCoord).rgb;
    vec3 SumLight = vec3(0.0);

    for (int i = 0; i < LightNum; i++) {
        vec3 LightDir = vec3(LightPos[i].xy - fragCoord.xy, LightPos[i].z);

        float D = length(LightDir);

        vec3 N = normalize(NormalMap * 2.0 - 1.0);
        vec3 L = normalize(LightDir);

        vec3 Diffuse = (LightColor[i].rgb * LightColor[i].a) * max(dot(N, L), 0.0);

        vec3 Ambient = vec3(0.0);

        float Attenuation = 
            1.0 / (Falloff.x + (Falloff.y * D) + (Falloff.z * D * D));

        vec3 Intensity = Ambient + Diffuse + Attenuation;
        vec3 FinalColor = DiffuseColor.rgb * Intensity;

        SumLight += FinalColor;
    }

    FragColor = vec4(SumLight, DiffuseColor.a);
};

/*
    vec4 normalMap = texture2D(normu, texCoord);
    vec3 normal = normalMap.xyz * 2.0 - 1.0;

    if (pixTex.a < 0.1f) 
		discard;

	vec4 shadeColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    float dist = length(light.xy - fragCoord.xy);
    if (dist < light.w) {
        float attenuation = clamp(1.0f - dist*dist/(light.w*light.w), 0.0f, 1.0f);
        attenuation *= attenuation;
        shadeColor += vec4(attenuation, attenuation, attenuation, 1.0f) * vec4(normal.xyz, 1);
        //shadeColor = vec4(1.0) + 0.1*normal;
        //shadeColor = vec4(1.0);
    }

    //FragColor = pixTex * shadeColor;
    FragColor = vec4(normal.xyz, 1);
}
*/

/*
    vec2 texLoc = vec2(texCoord.x, 1-texCoord.y);
	vec4 pixTex = texture2D(texture, texLoc);
    if (pixTex.a < 0.1f) 
		discard;
   
	vec4 shadeColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	if (lightImpact > 0.0f) {
		for (int i = 0; i < lightSize; i++) {
			vec2 loc = light[i].xy;
			float dist = length(loc - fragCoord.xy);
			if (dist < light[i].w) {
				float attenuation = clamp(1.0f - dist*dist/(light[i].w*light[i].w), 0.0f, 1.0f);
				attenuation *= attenuation;
				shadeColor += (vec4(attenuation, attenuation, attenuation, 0.0f) * vec4(lightColor[i])) * lightImpact;
			}
		}
	}
	shadeColor += ambientLight;
*/
