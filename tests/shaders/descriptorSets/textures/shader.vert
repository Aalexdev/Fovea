#version 450

// get the vertex attributes
layout (location = 0) in vec2 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 UV;

// the output to the fragment shader
layout (location = 0) out vec3 fragColor;
layout (location = 1) out vec2 fragUV;

void main(){
	fragColor = color;
	fragUV = UV;
    gl_Position = vec4(position, 0.0, 1.0);
}