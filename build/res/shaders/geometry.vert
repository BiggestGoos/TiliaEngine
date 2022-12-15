#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

out VS_OUT {
    vec3 color;
} vs_out;

void main()
{
    vs_out.color = aColor;
    gl_Position = projection * view * vec4(aPos.x, aPos.y, 0.0, 1.0);
}