#pragma once

#include "builders/InstanceBuilder.hpp"
#include "../vulkan/vulkan.h"

namespace Fovea{
	class Instance{
		public:
			Instance() = default;
			Instance(InstanceBuilder &builder){
				initialize(builder);
			}

			~Instance();

			Instance(const Instance &) = delete;
			Instance& operator=(const Instance &) = delete;

			void initialize(InstanceBuilder &builder);

			bool isValidationLayersEnabled() const;
			VkInstance getInstance() const;
			VkSurfaceKHR getSurface() const;

			std::vector<char*>& getValidationLayers();
		
		private:
			void checkValidationLayers(std::vector<const char *> &validationLayers);
			std::vector<const char*> getRequiredExtensions(void *window);
			void setDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
			void setupDebugMessenger();
			void checkRequiredExtensions(void *window);

			VkInstance instance;
			VkDebugUtilsMessengerEXT debugMessenger;
			VkSurfaceKHR surface;

			std::vector<char*> validationLayers;

			bool validationLayerEnabled;

	};
}