#version 430 core

layout(location = 0) in vec4 position;
uniform float czasokres;
uniform float czas;

void main(void)
{
float skalaCzasu = 3.14159f * 2.0f;

float czasCzasokres = mod(czas, skalaCzasu);
mat4 rotacja = mat4(cos(czasCzasokres), -sin(czasCzasokres), 0.0f, 0.0f,
                    sin(czasCzasokres), cos(czasCzasokres), 0.0f, 0.0f,
                    0.0f,              0.0f              , 0.0f, 0.0f,
                    0.0f,              0.0f              , 0.0f, 1.0f);
gl_Position = rotacja * position;
}