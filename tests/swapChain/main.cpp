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

Instance* createInstance(SDL_Window* window){
	InstanceBuilder builder;
	builder.enableValidationLayers(true);
	builder.setWindow(window);
	builder.requireValidationLayer("VK_LAYER_KHRONOS_validation");
	builder.enableValidationLayers();

	{
		uint32_t count = 0;
		SDL_Vulkan_GetInstanceExtensions(window, &count, nullptr);

		std::vector<const char*> extensions(count);
		SDL_Vulkan_GetInstanceExtensions(window, &count, extensions.data());

		for (auto &ext : extensions){
			builder.requireExtension(ext);
		}
	}
	 
	Instance* instance = nullptr;

	try{
		instance =  new Instance(builder);
	} catch (const char* err){
		fprintf(stderr, "ERROR | instance error : %s\n", err);
		exit(EXIT_FAILURE);
	}

	return instance;
}

PhysicalDevice* createPhysicalDevice(Instance* instance){
	PhysicalDeviceBuidler builder;
	builder.setInstance(instance);
	builder.requireFamily(FAMILY_GRAPHIC);
	builder.requireFamily(FAMILY_PRESENT);
	// builder.requireExtension(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

	PhysicalDevice* device = nullptr;
	
	try{
		device =  new PhysicalDevice(builder);
	} catch (const char* err){
		fprintf(stderr, "ERROR | physical device error : %s\n", err);
		exit(EXIT_FAILURE);
	}

	return device;
}

LogicalDevice* createLogicalDevice(PhysicalDevice* physicalDevice){
	LogicalDeviceBuilder builder;
	builder.setPhysicalDevice(physicalDevice);
	builder.requireQueue(FAMILY_PRESENT, 1);
	builder.requireQueue(FAMILY_GRAPHIC, 1);
	builder.requireExtension(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	
	LogicalDevice* device = nullptr;

	try{
		device =  new LogicalDevice(builder);
	} catch (const char* err){
		fprintf(stderr, "ERROR | logical device error : %s\n", err);
		exit(EXIT_FAILURE);
	}

	return device;
}

SwapChain* createSwapChain(LogicalDevice* device){
	SwapChainBuilder builder;
	builder.setDevice(device);
	
	VkExtent2D extent;
	extent.width = 1080;
	extent.height = 720;

	builder.setExtent(extent);
	builder.setRefreshMode(SwapChainRefreshMode::VSync);
	builder.setSwapchainBufferCount(2);

	SwapChain* swapChain = nullptr;
	try{
		swapChain = new SwapChain(builder);
	} catch (const char *err){
		fprintf(stderr, "ERROR | swapchain error : %s\n", err);
		exit(EXIT_FAILURE);
	}

	return swapChain;
}

int main(int argc, char** argv){

	initializeSDL();
	SDL_Window* window = initializeWindow();

	Instance* instance = createInstance(window);
	PhysicalDevice* physicalDevice = createPhysicalDevice(instance);
	LogicalDevice* device = createLogicalDevice(physicalDevice);
	SwapChain* swapChain = createSwapChain(device);

	printf("created swap chain with success !\n");
	SDL_Delay(3000);

	delete swapChain;
	delete device;
	delete physicalDevice;
	delete instance;
	
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;	
}