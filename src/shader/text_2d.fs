#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2D FontTextureSampler;
uniform vec4 ColorTint;

void main(){

	color = texture( FontTextureSampler, UV ) * ColorTint;
    // color = vec4(1, 1, 1, 1);
	
}