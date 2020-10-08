#version 330 core
layout (location=0) in vec3 pos;
layout (location=1) in vec4 inColor;

out vec4 color;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(pos, 1.0f);
    color = inColor;
}