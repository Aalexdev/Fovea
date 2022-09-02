#version 450

// get the vertex attributes
layout (location = 0) in vec2 position;
layout (location = 1) in vec3 color;

layout (push_constant) uniform Push{
	vec2 offset;
} push;

// the output to the fragment shader
layout (location = 0) out vec3 fragColor;

void main(){
	fragColor = color;
    gl_Position = vec4(position + push.offset, 0.0, 1.0);
}