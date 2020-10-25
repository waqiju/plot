#version 330 core

// Interpolated values from the vertex shaders
in vec4 color;
in vec2 UV;

// Ouput data
out vec4 finalColor;

// Values that stay constant for the whole mesh.
uniform sampler2D FontTextureSampler;
uniform vec4 ColorTint;

void main(){

	finalColor = texture( FontTextureSampler, UV ) * color * ColorTint;
	
}