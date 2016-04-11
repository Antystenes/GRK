#version 430 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

out vec4 FragColor;
uniform float czasokres;
uniform float czas;
uniform mat4 perspectiveMatrix;
 
out vec4 colorV;

void main(void)
{
FragColor = color;
float skalaCzasu = 3.14159f * 2.0f;

float czasCzasokres = mod(czas, skalaCzasu);
mat4 rotacja = mat4(cos(czasCzasokres), -sin(czasCzasokres), 0.0f, 0.0f,
                    sin(czasCzasokres), cos(czasCzasokres), 0.0f, 0.0f,
                    0.0f,              0.0f              , 1.0f, 0.0f,
                    0.0f,              0.0f              , 0.0f, 1.0f);
gl_Position = perspectiveMatrix * rotacja * (position + vec4(0.5f, 0.0f, 0.0f, 0.0f));
}
