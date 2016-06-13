#version 430 core

out vec3 color;
in vec3 posWorld;
in vec3 vnormal;
in vec3 texcoord;
in vec3 ray;
//in vec2 noise;
uniform vec3 camPos;
uniform sampler2D text;
const vec3 fogColor = vec3(0.1, 0.2, 0.35);

void main(void)
{
    vec3 waterColor = vec3(0.1, 0.3, 0.6);

    vec2 tc2 = 0.5*texcoord.xy + vec2(0.5);
    tc2.y += camPos.z;
    if(tc2.y > 0.33)
    {
        color = fogColor;
    }
    else
    {
        tc2.x = 0.5 + (tc2.x - 0.5) * (0.5/(0.5 - tc2.y));
        tc2.y = -1.0 + 0.5/(0.5 - tc2.y);
        color = mix(texture(text, tc2).rgb, fogColor, 1.0 - (1.0 - 0.5*tc2.y));
    }
}
