#version 450

// retrive the color from the vertex shader
layout (location = 0) in vec3 color;
layout (location = 1) in vec2 UV;

// output the color to the first color attachment
layout (location = 0) out vec4 outColor;

layout (set = 0, binding = 0) uniform sampler2D textures[1];

void main(){
	outColor = texture(textures[0], UV) * vec4(color, 1.0);
}