#include <Fovea/Fovea.hpp>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

#include <assert.h>
#include <vector>

using namespace Fovea;

void initializeSDL(){
	if (SDL_Init(SDL_INIT_VIDEO || SDL_INIT_EVENTS) != 0){
		fprintf(stderr, "! ERROR ! | SDL_Init Error : %s\n", SDL_GetError());
		exit(1);
	}
}

// basic window initialization with a vulkan flag
SDL_Window* initializeWindow(){
	SDL_Window* window = SDL_CreateWindow("vulkan instance test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1080, 720, SDL_WINDOW_VULKAN);
	if (window == nullptr){
		fprintf(stderr, "! ERROR ! | SDL_CreateWindow Error : %s\n", SDL_GetError());
		exit(1);
	}
	return window;
}

int main(int argc, char** argv){

	initializeSDL();
	SDL_Window* window = initializeWindow();

	InstanceBuilder builder;
	builder.enableValidationLayers(true);
	builder.setWindow(window);
	builder.requireValidationLayer("VK_LAYER_KHRONOS_validation");

	{
		uint32_t count = 0;
		SDL_Vulkan_GetInstanceExtensions(window, &count, nullptr);

		std::vector<const char*> extensions(count);
		SDL_Vulkan_GetInstanceExtensions(window, &count, extensions.data());

		for (auto &ext : extensions){
			builder.requireExtension(ext);
		}
	}
	
	Instance* instance = new Instance(builder);
	printf("instance initialized with success !");

	SDL_Delay(3000);

	delete instance;
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;	
}