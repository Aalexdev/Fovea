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
	infos[0].textureCount = 1;
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
	createInfo.tiling = FoveaImageTiling_Linear;
	texture = FoveaCreateTextureFromPath(filepath, &createInfo);
	return texture;
}

void loadTextures(FoveaTexture textures[]){
	textures[0] = loadTextureFromPath("texture.jpg");

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

	FoveaTexture textures[1];
	loadTextures(textures);
	FoveaDescriptorSet descriptorSet = createDescriptorSet(textures);
	FoveaShader shader = loadShader(descriptorSet);

	Vertex vertices[3];
	vertices[0].color = (vec3){1.f, 0.f, 0.f};
	vertices[0].position = (vec2){0.5f, 0.5f};
	vertices[0].UV = (vec2){1.f, 1.f};
	
	vertices[1].color = (vec3){0.f, 1.f, 0.f};
	vertices[1].position = (vec2){-0.5f, 0.5f};
	vertices[1].UV = (vec2){0.f, 1.f};

	vertices[2].color = (vec3){0.f, 0.f, 1.f};
	vertices[2].position = (vec2){0.5f, -0.5f};
	vertices[2].UV = (vec2){1.f, 0.f};

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

		FoveaSetTopology(FoveaTopology_Trigone);
		FoveaRenderTrigone(&vertices[0], &vertices[1], &vertices[2]);

		FoveaEndSwapChainRenderPass();
		FoveaEndFrame();
	}


	FoveaShutdown();
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}