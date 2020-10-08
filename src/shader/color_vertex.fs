#version 330 core
in vec4 color;
out vec4 finalColor;

uniform vec4 ColorTint;

void main()
{
    finalColor = ColorTint * color * vec4(1.0f, 1.0f, 1.0f, 1.0f);
}