#include <Fovea/Fovea.h>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include <stdio.h>
#include <stdlib.h>

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
		FoveaBeginSwapChainRenderPass();

		// at this stage, we cannot draw because we haven't loaded a shader

		FoveaEndSwapChainRenderPass();
		FoveaEndFrame();
	}


	FoveaShutdown();
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}