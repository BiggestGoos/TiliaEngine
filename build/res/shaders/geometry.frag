#version 330 core
out vec4 FragColor;

in vec3 fColor;

layout (std140) uniform color_block
{
    vec3 rgb;
    float multiplier_r;
    float multiplier_g;
    float multiplier_b;
};

void main()
{
    FragColor = vec4(fColor, 1.0);   
    //FragColor = vec4(fColor * vec3(rgb.r * multiplier_r, rgb.g * multiplier_g, rgb.b * multiplier_b), 1.0);   
}