#include "Fovea/builders/LogicalDeviceBuilder.hpp"

namespace Fovea{
	void LogicalDeviceBuilder::requireExtension(const char *extension){
		requiredExtensions.push_back(extension);
	}

	void LogicalDeviceBuilder::requireQueue(QueueFamily family, uint32_t count){
		requiredQueuesCount[static_cast<size_t>(family)] = count;
		queuePriorities[static_cast<size_t>(family)].resize(count);
		
		for (auto &p : queuePriorities[static_cast<size_t>(family)]){
			p = 0.f;
		}
	}

	void LogicalDeviceBuilder::setQueuePriority(QueueFamily family, uint32_t index, float priority){
		queuePriorities[static_cast<size_t>(family)][index] = priority;
	}

	void LogicalDeviceBuilder::setPhysicalDevice(PhysicalDevice *physicalDevice){
		this->physicalDevice = physicalDevice;
	}

	void LogicalDeviceBuilder::setCommandPoolFlags(VkCommandPoolCreateFlags flags, QueueFamily family){
		commandPoolFlags[family] = flags;
	}
}