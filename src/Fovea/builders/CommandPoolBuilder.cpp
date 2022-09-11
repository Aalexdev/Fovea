#include "Fovea/builders/CommandPoolBuilder.hpp"

namespace Fovea{
	void CommandPoolBuilder::setFamily(QueueFamily family){
		this->family = family;
	}

	void CommandPoolBuilder::setFlag(VkCommandPoolCreateFlags flags){
		this->flags = flags;
	}

	void CommandPoolBuilder::setLogicalDevice(LogicalDevice* logicalDevice){
		this->device = logicalDevice;
	}
}