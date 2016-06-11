#version 430 core

layout(location = 0) in vec3 position;

uniform mat4 transformation;
uniform mat4 camera;

void main(void)
{
//vL = normalize (position - light);
//mat4 normalMatrix = transpose(inverse(modelToCameraMatrix));
//vNormal = normalize(normalMatrix * position);

gl_Position = /*perspectiveMatrix */ camera * transformation * vec4(position, 1);
}
