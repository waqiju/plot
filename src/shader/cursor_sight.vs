#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec2 inViewportPosition;
layout(location = 1) in float inDirection;

out vec2 viewportPosition;
out float direction;

void main()
{
    gl_Position =  vec4(inViewportPosition,0,1);
    viewportPosition = inViewportPosition;
    direction = inDirection;
}

