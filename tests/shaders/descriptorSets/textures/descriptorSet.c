#include "Fovea/Fovea.h"

#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct vec2{
	float x, y;
} vec2;

typedef struct vec3{
	float x, y, z;
} vec3;

// the structure that contain all the data for each vertices
typedef struct Vertex{
	vec2 position;
	vec3 color;
	vec2 UV;
} Vertex;

FoveaShader loadShader(FoveaDescriptorSet descriptorSet){
	FoveaShader shader;
	
	FoveaShaderCreateInfo createInfo;

	// copy the default parameters
	FoveaDefaultShaderCreateInfo(&createInfo);

	createInfo.vertexFilepath = "shader.vert.spv";
	createInfo.fragmentFilepath = "shader.frag.spv";

	createInfo.vertexInputSize = sizeof(Vertex);
	createInfo.descriptorSetCount = 1;
	createInfo.descriptorSets = &descriptorSet;

	// the data for each members of Vertex
	FoveaShaderVertexAttribute attributes[3];

	attributes[0].format = FoveaFormat_Float_vec2;
	attributes[0].offset = offsetof(Vertex, position);

	attributes[1].format = FoveaFormat_Float_vec3;
	attributes[1].offset = offsetof(Vertex, color);

	attributes[2].format = FoveaFormat_Float_vec2;
	attributes[2].offset = offsetof(Vertex, UV);

	createInfo.vertexAttributes = attributes;
	createInfo.vertexAttributsCount = 3;

	shader = FoveaCreateShader(&createInfo);
	return shader;
}

FoveaDescriptorSet createDescriptorSet(FoveaTexture textures[]){
	FoveaDescriptorSet descriptorSet;
	FoveaDescriptorSetCreateInfo createInfo;
	
	createInfo.setCount = 1;
	createInfo.descriptorCount = 1;

	FoveaDescriptorInfo infos[1];

	// set which stage of the shader the descriptor set will be sent
	// in this case, since we are sending texture, only the fragment shader will need them
	infos[0].stage = FoveaShaderStage_Fragment;

	// the type of the descriptor set, can be buffer or texture
	infos[0].type = FoveaDescriptorType_Texture;

	// set the textures
	infos[0].textureCount = 2;
	infos[0].textures = textures;

	// and then the binding of the descriptor set
	infos[0].binding = 0;

	createInfo.descriptors = infos;

	descriptorSet = FoveaCreateDescriptorSet(&createInfo);
	return descriptorSet;
}

FoveaTexture loadTextureFromPath(const char *filepath){
	FoveaTexture texture;
	FoveaTextureCreateInfo createInfo = FoveaDefaultTextureCreateInfo();
	createInfo.anisotropy = Fovea_False;
	texture = FoveaCreateTextureFromPath(filepath, &createInfo);
	return texture;
}

#define DEMO_TEXTURE_W 255
#define DEMO_TEXTURE_H 255

FoveaTexture loadTextureFromData(){
	FoveaTexture texture;
	FoveaTextureCreateInfo createInfo = FoveaDefaultTextureCreateInfo();

	unsigned char* data = (unsigned char*)malloc(sizeof(unsigned char) * DEMO_TEXTURE_W * DEMO_TEXTURE_H * 4);

	if (!data){
		fprintf(stderr, "MALLOC ERROR\n");
		exit(1);
	}

	for (uint32_t x = 0; x<DEMO_TEXTURE_W; x++){
		for (uint32_t y = 0; y<DEMO_TEXTURE_H; y++){
			uint32_t i = (x * DEMO_TEXTURE_W + y) * 4;

			unsigned char* r = &data[i];
			unsigned char* g = &data[i+1];
			unsigned char* b = &data[i+2];
			unsigned char* a = &data[i+3];

			*r = x;
			*g = y;
			*b = 0;
			*a = 255;
		}
	}

	texture = FoveaCreateTextureFromData(FoveaImageFormat_R8G8B8A8, (FoveaUIVec2){DEMO_TEXTURE_W, DEMO_TEXTURE_H}, data, &createInfo);
	free(data);

	return texture;
}

void loadTextures(FoveaTexture textures[]){
	textures[0] = loadTextureFromPath("texture.jpg");
	textures[1] = loadTextureFromData();
}

int main(int argc, char **argv){

	// initizalize SDL
	if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO) != 0){
		fprintf(stderr, "failed to initialize SDL2 : %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	// create the window
	// /!\ make sure to initialize it with the fovea window flag
	SDL_Window* window = SDL_CreateWindow("Fovea initialize", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1080, 720, SDL_WINDOW_FOVEA);

	if (window == NULL){
		fprintf(stderr, "failed to create the window : %s\n", SDL_GetError());
		SDL_Quit();
		return EXIT_FAILURE;
	}

	FoveaInitialize(window);

	FoveaSetVertexSize(sizeof(Vertex));

	FoveaTexture textures[2];
	loadTextures(textures);
	FoveaDescriptorSet descriptorSet = createDescriptorSet(textures);
	FoveaShader shader = loadShader(descriptorSet);

	Vertex vertices[8];

	/*

	this code is very ungly and repetitive, you shouldn't program like this but i made it so it is easier to understant

	*/

	// quad 1
	vertices[0].color = (vec3){1.f, 1.f, 1.f};
	vertices[0].position = (vec2){0.f, 0.5f};
	vertices[0].UV = (vec2){1.f, 1.f};
	
	vertices[1].color = (vec3){1.f, 1.f, 1.f};
	vertices[1].position = (vec2){0.f, -0.5f};
	vertices[1].UV = (vec2){1.f, 0.f};
	vertices[2].color = (vec3){1.f, 1.f, 1.f};
	vertices[2].position = (vec2){-1.f, -0.5f};
	vertices[2].UV = (vec2){0.f, 0.f};
	
	vertices[3].color = (vec3){1.f, 1.f, 1.f};
	vertices[3].position = (vec2){-1.f, 0.5f};
	vertices[3].UV = (vec2){0.f, 1.f};

	// quad 2
	vertices[4].color = (vec3){1.f, 1.f, 1.f};
	vertices[4].position = (vec2){1.f, 0.5f};
	vertices[4].UV = (vec2){1.f, 1.f};
	
	vertices[5].color = (vec3){1.f, 1.f, 1.f};
	vertices[5].position = (vec2){1.f, -0.5f};
	vertices[5].UV = (vec2){1.f, 0.f};

	vertices[6].color = (vec3){1.f, 1.f, 1.f};
	vertices[6].position = (vec2){0.f, -0.5f};
	vertices[6].UV = (vec2){0.f, 0.f};
	
	vertices[7].color = (vec3){1.f, 1.f, 1.f};
	vertices[7].position = (vec2){0.f, 0.5f};
	vertices[7].UV = (vec2){0.f, 1.f};

	FoveaBool launched = Fovea_True;
	while (launched){
		// poll events
		{
			SDL_Event e;
			while (SDL_PollEvent(&e)){
				switch (e.type){
					case SDL_QUIT:
						launched = Fovea_False;
						break;
					
					case SDL_WINDOWEVENT:
						switch (e.window.event){
							case SDL_WINDOWEVENT_RESIZED:
								FoveaOnWindowResized((uint32_t)e.window.data1, (uint32_t)e.window.data2);
								break;
							
							case SDL_WINDOWEVENT_MINIMIZED:
								FoveaOnWindowMinimized();
								break;
						}
						break;
				}
			}
		}

		FoveaBeginFrame();

		// the swapchain is the name of the screen render target, once you called this function, every draw calls will be drawn onto the screen, until you call FoveaEndSwapChainRenderPass()
		FoveaBeginSwapChainRenderPass();

		uint32_t indices[] = {0};
		FoveaUseShader(shader, indices);

		FoveaSetTopology(FoveaTopology_Quad);
		FoveaRenderQuad(&vertices[0], &vertices[1], &vertices[2], &vertices[3]);
		FoveaRenderQuad(&vertices[4], &vertices[5], &vertices[6], &vertices[7]);

		FoveaEndSwapChainRenderPass();
		FoveaEndFrame();
	}


	FoveaShutdown();
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}