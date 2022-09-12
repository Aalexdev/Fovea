#pragma once

#include "builders/LogicalDeviceBuilder.hpp"
#include "../vulkan/vulkan.h"

#include <array>

namespace Fovea{
	class CommandPool; // create a prototype to avoid compilation error

	class LogicalDevice{
		public:
			LogicalDevice() = default;
			LogicalDevice(LogicalDeviceBuilder &builder){
				initialize(builder);
			}
			~LogicalDevice();

			LogicalDevice(const LogicalDevice &) = delete;
			LogicalDevice& operator=(const LogicalDevice &) = delete;

			void initialize(LogicalDeviceBuilder &builder);

			VkDevice getDevice() const;
			VkQueue getQueue(QueueFamily family, uint32_t index);
			PhysicalDevice* getPhysicalDevice();
			Instance* getInstance();


			void createImageWithInfo(const VkImageCreateInfo &imageInfo, VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &imageMemory);
			void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer,VkDeviceMemory &bufferMemory);
			CommandPool* getCommandPool(QueueFamily family);

		private:
			void createCommandPools(LogicalDeviceBuilder &builder);

			PhysicalDevice* physicalDevice = nullptr;
			VkDevice device = VK_NULL_HANDLE;

			struct QueueType{
				VkQueue* queues = nullptr;
				uint32_t queueCount = 0;
			};

			QueueType queues[FAMILY_COUNT];
			CommandPool* commandPools[FAMILY_COUNT];
	};
}