#include <Fovea/Fovea.hpp>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

#include <glm/glm.hpp>

#include <assert.h>
#include <vector>

using namespace Fovea;
using namespace Fovea::renderer;

struct PushConstant{
	glm::vec2 offset = glm::vec2(0.f);
};

void initializeSDL(){
	if (SDL_Init(SDL_INIT_VIDEO || SDL_INIT_EVENTS) != 0){
		fprintf(stderr, "! ERROR ! | SDL_Init Error : %s\n", SDL_GetError());
		exit(1);
	}
}

// basic window initialization with a vulkan flag
SDL_Window* initializeWindow(){
	SDL_Window* window = SDL_CreateWindow("vulkan instance test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1080, 720, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
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
		exit(1);
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
		exit(1);
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
		exit(1);
	}

	return device;
}

BasicRenderer* createRenderer(LogicalDevice* device){
	BasicRendererBuilder builder;

	builder.setDevice(device);
	builder.setBufferCount(2);
	builder.setExtent({1080, 720});
	builder.setRefreshMode(SwapChainRefreshMode::VSync);

	BasicRenderer* renderer = nullptr;
	try{
		renderer = new BasicRenderer(builder);
	} catch (const char *err){
		fprintf(stderr, "ERROR | renderer error : %s\n", err);
		exit(1);
	}

	return renderer;
}

Pipeline* createPipeline(LogicalDevice* device, BasicRenderer* renderer, const std::filesystem::path& folderPath){
	PipelineBuilder builder;

	builder.setDevice(device);
	builder.setRenderPass(renderer->getSwapChainRenderPass());
	builder.setShaderStage(SHADER_STAGE_VERTEX, (folderPath / "shader.vert.spv").string().c_str());
	builder.setShaderStage(SHADER_STAGE_FRAGMENT, (folderPath / "shader.frag.spv").string().c_str());
	builder.setPushConstant<PushConstant>();

	Pipeline* pipeline = nullptr;
	try{
		pipeline = new Pipeline(builder);
	} catch (const char* err){
		fprintf(stderr, "ERROR | pipeline error : %s\n", err);
		exit(1);
	}

	return pipeline;
}

int main(int argc, char** argv){
	std::filesystem::path path = argv[0];
	path = path.parent_path();

	initializeSDL();
	SDL_Window* window = initializeWindow();

	Instance* instance = createInstance(window);
	PhysicalDevice* physicalDevice = createPhysicalDevice(instance);
	LogicalDevice* device = createLogicalDevice(physicalDevice);
	BasicRenderer* renderer = createRenderer(device);
	Pipeline* pipeline = createPipeline(device, renderer, path / "shaders/");

	bool launched = true;
	VkClearColorValue clearColor;

	PushConstant push;

	while (launched){
		SDL_Event e;
		while (SDL_PollEvent(&e)){
			switch (e.type){
				case SDL_QUIT:
					launched = false;
					break;

				case SDL_WINDOWEVENT:
					switch (e.window.event){
						case SDL_WINDOWEVENT_MINIMIZED:
							renderer->onWindowMinimized();
							break;
						
						case SDL_WINDOWEVENT_RESIZED:
							renderer->onWindowResied({static_cast<uint32_t>(e.window.data1), static_cast<uint32_t>(e.window.data2)});
							break;
					}
					break;
			}
		}

		VkCommandBuffer commandBuffer = renderer->beginFrame();
		if (commandBuffer){
			clearColor.float32[0] = SDL_sinf(SDL_GetTicks64() / 1000.f);
			clearColor.float32[1] = SDL_sinf(SDL_GetTicks64() / 2000.f);
			clearColor.float32[2] = SDL_sinf(SDL_GetTicks64() / 3000.f);
			clearColor.float32[3] = 1.f;

			push.offset.x = SDL_sinf(SDL_GetTicks64() / 1000.f);

			renderer->setClearColor(clearColor);

			renderer->beginSwapChainRenderPass();

			pipeline->bindPushConstant(commandBuffer, &push);
			pipeline->bind(commandBuffer, nullptr);
			vkCmdDraw(commandBuffer, 3, 1, 0, 0);

			renderer->endSwapChainRenderPass(commandBuffer);

			renderer->endFrame();
		}
	}

	delete pipeline;
	delete renderer;
	delete device;
	delete physicalDevice;
	delete instance;
	
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;	
}