#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNor;
layout (location = 2) in vec2 auvs;

uniform mat4 mvp;
uniform mat4 mv;
uniform mat4 mv_n;

out vec2 uv;
out vec3 n;
out vec3 pos;

void main()
{
   vec4 tmp_pos = mv*vec4(aPos, 1);
   pos = tmp_pos.xyz/tmp_pos.w;
   gl_Position = mvp*vec4(aPos, 1.0);
   //gl_Position = vec4((mvp*vec4(aPos.x, aPos.y, aPos.z, 1.0)).xyz, 1);
   uv = auvs;
   vec4 n_ = mv_n*vec4(aNor, 0);
   n = n_.xyz/n_.w;
   n = normalize(n_.xyz);

   vec4 camera = inverse(mvp)*vec4(0, 0, 0, 1);
   //cameraPos = camera.xyz/camera.w;
};