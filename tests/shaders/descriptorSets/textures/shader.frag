#version 460 core

// retrive the color from the vertex shader
layout (location = 0) in vec3 color;
layout (location = 1) in vec2 UV;

// output the color to the first color attachment
layout (location = 0) out vec4 outColor;

layout (set = 0, binding = 0) uniform sampler2D textures[2];

void main(){
	vec4 c = vec4(0.0);

	if (int(UV.x * 100.0) % 2 == 0){
		if (int(UV.y * 100.0) % 2 == 0){
			c = texture(textures[0], UV);
		} else {
			c = texture(textures[1], UV);
		}
	} else {
		if (int(UV.y * 100.0) % 2 == 1){
			c = texture(textures[0], UV);
		} else {
			c = texture(textures[1], UV);
		}
	}

	outColor = c * vec4(color, 1.0);
}
