#version 430 core

// Caustics
// Original code Copyright (c) Nvidia Corporation. All Rights Reserved.
// The Shader is based on the original work by Daniel Sanchez-Crespo
// of the Universitat Pompeu Fabra, Barcelona, Spain.

#define VTXSIZE 0.01f  // Amplitude
#define WAVESIZE 10.0f // Frequency
#define FACTOR 1.0f
#define SPEED 2.0f
#define OCTAVES 5

out vec4 color;
in vec4 Position;
uniform sampler2D LightMap;
uniform sampler2D GroundMap;
in uniform float Timer;
in vec2 texcoord;

// Derivative of wave function. Returns df/dx and df/dy partial derivatives
vec2 gradwave(float x, float y, float timer)
{
    float dZx = 0.0f;
    float dZy = 0.0f;
    float octaves = OCTAVES;
    float factor = FACTOR;
    float d = sqrt(x*x + y*y); //distance
    float inside;

    do
    {
        inside = timer * SPEED + (1/factor) * x * y * WAVESIZE;
        dZx += d * sin(inside) * y * WAVESIZE - factor * cos(inside) * x/d;
        dZy += d * sin(inside) * x * WAVESIZE - factor * cos(inside) * y/d;
        factor = factor / 2;
        octaves--;
    } while (octaves > 0);

    return vec2(2 * VTXSIZE * dZx, 2 * VTXSIZE * dZy);
}

vec3 line_plane_intercept(vec3 lineP, vec3 lineN, vec3 planeN, float planeD)
{
    // This implementation assumes that planeN is always pointing up
    float dst = (planeD - lineP.z) /lineN.z; //distance
    return lineP + lineN * dst;
}

void main(void)
{
    vec2 dxdy = gradwave(Position.x, Position.y, Timer);

    vec3 intercept = line_plane_intercept(Position.xyz, vec3(dxdy, clamp(Position.w, 0.0, 1.0)), vec3(0,0,1), -0.8);

    color.rgb = vec3(texture(LightMap, intercept.xy * 0.8));
    color.rgb += vec3(texture(GroundMap, Position.xy));
    color.a = 1;

    //color = texture(GroundMap, texcoord).rgba;
}
