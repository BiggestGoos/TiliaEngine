#version 330 core
layout (location = 0) in vec3 aPos;

mat4 projection;
mat4 view;

void main()
{
    gl_Position = projection * view * vec4(aPos, 1.0);
}