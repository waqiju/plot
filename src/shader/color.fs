#version 330 core
out vec4 finalColor;

uniform vec4 Color;

void main()
{
    finalColor = Color * vec4(1.0f, 1.0f, 1.0f, 1.0f);
}