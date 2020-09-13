#version 330 core
out vec4 finalColor;

uniform vec3 Color;

void main()
{
    finalColor = vec4(Color, 1.0f);
    // finalColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}