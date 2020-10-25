#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 pos;
layout(location=1) in vec4 inColor;
layout(location = 2) in vec2 vertexUV;

// Output data ; will be interpolated for each fragment.
out vec4 color;
out vec2 UV;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(pos, 1.0f);
    
    // UV of the vertex. No special space for this one.
    color = inColor;
    UV = vertexUV;
}

