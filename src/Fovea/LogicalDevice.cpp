#include "Fovea/LogicalDevice.hpp"
#include "Fovea/CommandPool.hpp"

#include <stdexcept>
#include <cassert>

namespace Fovea{

	struct UniqueFamily : public PhysicalDevice::FamilyDetails{
		UniqueFamily(const PhysicalDevice::FamilyDetails &other) : PhysicalDevice::FamilyDetails(other){};
		UniqueFamily() = default;

		std::vector<float> priorities{};
	};

	void LogicalDevice::createCommandPools(LogicalDeviceBuilder &builder){
		auto families = physicalDevice->getEnabledFamilies();

		for (int i=0; i<FAMILY_COUNT; i++){
			if (families[i]){
				CommandPool* pool = new CommandPool();
				CommandPoolBuilder b;
				b.setFamily(static_cast<QueueFamily>(i));
				b.setLogicalDevice(this);
				b.setFlag(builder.commandPoolFlags[i]);
				pool->initialize(b);
				commandPools[i] = pool;
			} else {
				commandPools[i] = nullptr;
			}
		}
	}

	void LogicalDevice::initialize(LogicalDeviceBuilder &builder){
		assert(builder.physicalDevice != nullptr && "cannot create a logical device without a valid physical device");
		physicalDevice = builder.physicalDevice;
		Instance* instance = physicalDevice->getInstance();

		std::vector<PhysicalDevice::FamilyDetails> families = physicalDevice->getFamilies();
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

		std::vector<UniqueFamily> uniqueFamilies;

		for (UniqueFamily family : families){
			bool isUnique = true;
			queues[static_cast<size_t>(family.type)].queueCount = builder.requiredQueuesCount[static_cast<size_t>(family.type)];

			family.priorities = builder.queuePriorities[static_cast<size_t>(family.type)];

			for (auto &uniqueFamilie : uniqueFamilies){
				if (uniqueFamilie.family == family.family){

					uniqueFamilie.priorities.insert(uniqueFamilie.priorities.end(), family.priorities.begin(), family.priorities.end());

					isUnique = false;
					break;
				}
			}

			if (isUnique) uniqueFamilies.push_back(family);
		}

		for (int i=0; i<FAMILY_COUNT; i++){
			if (queues[i].queueCount == 0) continue;
			queues[i].queues = new VkQueue[queues[i].queueCount];
		}

		std::vector<float> priorities(uniqueFamilies.size());

		for (int i=0; i<uniqueFamilies.size(); i++){
			auto& queueFamily = uniqueFamilies[i];

			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily.family;

			queueCreateInfo.queueCount = static_cast<uint32_t>(queueFamily.priorities.size());
			queueCreateInfo.queueFamilyIndex = queueFamily.family;
			queueCreateInfo.pQueuePriorities = queueFamily.priorities.data();
			
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures features = physicalDevice->getEnabledFeatures();

		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		
		createInfo.pEnabledFeatures = &features;

		createInfo.enabledExtensionCount = static_cast<uint32_t>(builder.requiredExtensions.size());
		createInfo.ppEnabledExtensionNames = builder.requiredExtensions.data();

		if (instance->isValidationLayersEnabled()){
			createInfo.enabledLayerCount = static_cast<uint32_t>(instance->getValidationLayers().size());
			createInfo.ppEnabledLayerNames = instance->getValidationLayers().data();
		} else {
			createInfo.enabledLayerCount = 0;
		}

		if (vkCreateDevice(physicalDevice->getDevice(), &createInfo, nullptr, &device) != VK_SUCCESS){
			throw "failed to create the logical device";
		}

		uint32_t queueIndices[FAMILY_COUNT];
		for (int i=0; i<FAMILY_COUNT; i++){
			queueIndices[i] = 0;
		}

		for (auto &f : families){
			uint32_t *queueIndex = nullptr;

			for (auto &uf : uniqueFamilies){
				if (f.family == uf.family){
					queueIndex = &queueIndices[static_cast<int>(uf.type)];
				}
			}

			for (int i=0; i<builder.requiredQueuesCount[static_cast<int>(f.type)]; i++){
				vkGetDeviceQueue(device, f.family, *queueIndex, &queues[static_cast<size_t>(f.type)].queues[i]);
				(*queueIndex)++;
			}
		}

		createCommandPools(builder);
	}

	LogicalDevice::~LogicalDevice(){
		vkDestroyDevice(device, nullptr);

		for (int i=0; i<FAMILY_COUNT; i++){
			if (queues[i].queueCount > 0){
				delete[] queues[i].queues;
			}
		}
	}

	VkDevice LogicalDevice::getDevice() const{
		return device;
	}

	void LogicalDevice::createImageWithInfo(const VkImageCreateInfo &imageInfo, VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &imageMemory){
		if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
			throw "failed to create image !";
		}

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(device, image, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = physicalDevice->findMemoryType(memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
			throw "failed to allocate image memory";
		}

		if (vkBindImageMemory(device, image, imageMemory, 0) != VK_SUCCESS) {
			throw "failed to bind image memory";
		}
	}

	void LogicalDevice::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer,VkDeviceMemory &bufferMemory){
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
			throw "failed to create buffer";
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = physicalDevice->findMemoryType(memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
			throw "failed to allocate buffer memory";
		}

		vkBindBufferMemory(device, buffer, bufferMemory, 0);
	}

	VkQueue LogicalDevice::getQueue(QueueFamily family, uint32_t index){
		return queues[family].queues[index];
	}

	Instance* LogicalDevice::getInstance(){
		return physicalDevice->getInstance();
	}

	CommandPool* LogicalDevice::getCommandPool(QueueFamily family){
		return commandPools[family];
	}
}