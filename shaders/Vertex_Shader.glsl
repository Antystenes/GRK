#version 430 core

layout(location = 0) in vec3 position;

uniform mat4 transformation;
//in vec4 light;
//uniform mat4 perspectiveMatrix;
//uniform mat4 modelToCameraMatrix;
//out vec4 vNormal;
//out vec4 vL;

void main(void)
{
//vL = normalize (position - light);
//mat4 normalMatrix = transpose(inverse(modelToCameraMatrix));
//vNormal = normalize(normalMatrix * position);

gl_Position = /*perspectiveMatrix * modelToCameraMatrix */ transformation * vec4(position, 1);
}
