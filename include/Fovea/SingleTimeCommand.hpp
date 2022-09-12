#pragma once

#include "../vulkan/vulkan.h"
#include "LogicalDevice.hpp"
#include "CommandPool.hpp"

namespace Fovea{
	class SingleTimeCommand{
		public:
			SingleTimeCommand(LogicalDevice* device, QueueFamily family, uint32_t queueIndex);
			~SingleTimeCommand();

			SingleTimeCommand(const SingleTimeCommand &) = delete;
			SingleTimeCommand& operator=(const SingleTimeCommand &) = delete;

			void begin();
			void end();
			VkCommandBuffer getCommandBuffer();

		private:
			VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
			VkQueue queue = VK_NULL_HANDLE;
			LogicalDevice* device = nullptr;
			CommandPool* commandPool = nullptr;

	};
}