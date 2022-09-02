#version 450

// retrive the color from the vertex shader
layout (location = 0) in vec3 color;

// output the color to the first color attachment
layout (location = 0) out vec4 outColor;

void main(){
	outColor = vec4(color, 1.0);
}