#version 130

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
uniform vec4 AmbientLight;
uniform int Flipped;

void main()
{
    // Quadratic light falloff
    vec3 Falloff = vec3(0.1, 0.2, 0.0);

    vec4 DiffuseColor = texture2D(textu, texCoord);

    if (DiffuseColor.a < 0.1f)
        discard;

    vec3 NormalMap = texture2D(normu, texCoord).rgb;
    vec3 SumLight = vec3(0.0);

    for (int i = 0; i < LightNum; i++) {
        vec3 LightDir = vec3(LightPos[i].xy - fragCoord.xy, LightPos[i].z);
        if (Flipped == 1)
            LightDir.x = -LightDir.x;

        float D = length(LightDir);

        vec3 N = normalize(NormalMap * 2.0 - 1.0);
        vec3 L = normalize(LightDir);

        vec3 Diffuse = 
            (LightColor[i].rgb * LightColor[i].a) * max(dot(N, L), 0.0);

        vec3 Ambient = AmbientLight.rgb * AmbientLight.a;

        float Attenuation = 
            1.0 / (Falloff.x + (Falloff.y * D) + (Falloff.z * D * D));

        vec3 Intensity = Ambient + Diffuse + Attenuation;
        vec3 FinalColor = DiffuseColor.rgb * Intensity;

        SumLight += FinalColor;
    }

    FragColor = vec4(SumLight, DiffuseColor.a);
};
