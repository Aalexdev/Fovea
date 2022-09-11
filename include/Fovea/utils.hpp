#pragma once

#include "../vulkan/vulkan.h"
#include "enum.hpp"

namespace Fovea{
	void transitionImageLayout(VkCommandBuffer commandBuffer, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	void copyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	VkQueueFlagBits familyQueueToVkQueue(QueueFamily family);
}