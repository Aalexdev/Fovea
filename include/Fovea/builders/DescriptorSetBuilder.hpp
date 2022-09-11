#pragma once

#include "../../vulkan/vulkan.h"
#include "../LogicalDevice.hpp"

#include <vector>

namespace Fovea{
	class DescriptorSetBuilder{
		friend class DescriptorSet;
		public:
			struct Descriptor{
				friend class DescriptorSet;

				uint32_t binding;
				VkDescriptorType type;
				VkPipelineStageFlags stage;
				std::vector<VkDescriptorImageInfo> imageInfos;
				uint32_t imageCount;
				uint32_t bufferSize;

				private:
					VkDescriptorBufferInfo bufferInfo;
			};

			void setDescriptors(const std::vector<Descriptor> &descriptors);
			void setDescriptorSetCount(uint32_t count);
			void setLogicalDevice(LogicalDevice* device);

		private:
			LogicalDevice* device = nullptr;
			std::vector<Descriptor> descriptors;
			uint32_t descriptorSetCount = 1;
	};
}