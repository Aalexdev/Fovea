#pragma once

#include "builders/CommandPoolBuilder.hpp"
#include "LogicalDevice.hpp"
#include "enum.hpp"
#include "../vulkan/vulkan.h"

namespace Fovea{
	class CommandPool{
		public:
			CommandPool() = default;
			~CommandPool();

			// destroy copy
			CommandPool(const CommandPool &) = delete;
			CommandPool &operator=(const CommandPool &) = delete;

			void initialize(CommandPoolBuilder &builder);

			VkCommandPool getCommandPool();
			QueueFamily getFamily();

		private:
			LogicalDevice* device;
			VkCommandPool commandPool = VK_NULL_HANDLE;
			QueueFamily family;
			int flags;
	};
}