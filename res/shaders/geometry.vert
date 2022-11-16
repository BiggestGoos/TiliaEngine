#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

layout (std140) uniform block
{
    float scalar;
    mat2x3 matrix_2x3;
    mat4 matrix_4;
    vec3 rgb;
    float arr[5];
};

out VS_OUT {
    vec3 color;
} vs_out;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    //vs_out.color = aColor * rgb;
    vs_out.color = vec3(arr[0], arr[1], arr[2]);
    gl_Position = projection * view * matrix_4 * vec4(aPos.x, aPos.y, 0.0, 1.0) * scalar; 
}