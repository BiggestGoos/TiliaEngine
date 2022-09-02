#version 330 core
out vec4 FragColor;

void main()
{
    
    float alpha = 0.5;

    FragColor = vec4(1.0, 1.0, 1.0, alpha); // set all 4 vector values to 1.0
}