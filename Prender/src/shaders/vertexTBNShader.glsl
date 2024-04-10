#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNor;

uniform mat4 mv;
uniform mat4 p;

out vec4 n;

void main()
{
   gl_Position = mv*vec4(aPos, 1.0);
   n = mv*vec4(aNor, 0);
};