#include "Fovea/CommandPool.hpp"
#include "Fovea/utils.hpp"

#include <stdexcept>
#include <cassert>

namespace Fovea{
	CommandPool::~CommandPool(){
		if (commandPool) vkDestroyCommandPool(device->getDevice(), commandPool, nullptr);
	}

	void CommandPool::initialize(CommandPoolBuilder &builder){
		assert(builder.device != nullptr && "cannot create a command pool without a valid logical device");
		assert(builder.family != FAMILY_NONE && "cannot create a command pool without a defined family");

		device = builder.device;
		flags = builder.flags;
		family = builder.family;
		
		VkCommandPoolCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		createInfo.queueFamilyIndex = device->getPhysicalDevice()->getFamily(family).family;
		createInfo.flags = flags;

		if (vkCreateCommandPool(device->getDevice(), &createInfo, nullptr, &commandPool)){
			throw "failed to create a command pool";
		}
	}

	VkCommandPool CommandPool::getCommandPool(){
		return commandPool;
	}

	QueueFamily CommandPool::getFamily(){
		return family;
	}
}