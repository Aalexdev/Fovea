#pragma once

#include "DescriptorSetLayout.hpp"
#include "DescriptorPool.hpp"
#include "LogicalDevice.hpp"

#include "../vulkan/vulkan.h"

#include <vector>

namespace Fovea{
	class DescriptorWriter{
		public:
			DescriptorWriter(LogicalDevice* device, DescriptorSetLayout* setLayout, DescriptorPool* pool){
				initialize(device, setLayout, pool);
			}

			void initialize(LogicalDevice* device, DescriptorSetLayout* setLayout, DescriptorPool* pool);
			
			void writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo);
			void writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo);
			void writeImages(uint32_t binding, uint32_t imageCount, VkDescriptorImageInfo *imageInfos);
			
			bool build(VkDescriptorSet &set);
			void overwrite(VkDescriptorSet &set);

		private:
			LogicalDevice* device = nullptr;
			DescriptorSetLayout* setLayout = nullptr;
			DescriptorPool* pool = nullptr;
			std::vector<VkWriteDescriptorSet> writes{};
	};
}