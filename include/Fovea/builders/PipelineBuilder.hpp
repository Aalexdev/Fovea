#pragma once

#include "../../vulkan/vulkan.h"
#include "../enum.hpp"
#include "../LogicalDevice.hpp"

#include <vector>
#include <filesystem>

namespace Fovea{
	struct PipelineConfigInfo{
		PipelineConfigInfo(){defaultPipelineConfigInfo(*this);}
		static void defaultPipelineConfigInfo(PipelineConfigInfo &configInfo);

		VkPipelineViewportStateCreateInfo viewportInfo{};
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
		VkPipelineRasterizationStateCreateInfo rasterizationInfo{};
		VkPipelineMultisampleStateCreateInfo multisampleInfo{};
		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo{};
		std::vector<VkDynamicState> dynamicStateEnables{};
		VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
		VkRenderPass renderPass = VK_NULL_HANDLE;
		uint32_t subpass = 0;
	};

	struct PipelineVertexDescription{
		VkVertexInputBindingDescription bindingDescription;
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};
	};

	class Pipeline;
	class DescriptorSet;

	class PipelineBuilder{
		friend class Pipeline;
		public:
			PipelineBuilder();
			PipelineConfigInfo* operator->() {return &config;}

			void setDevice(LogicalDevice* device);
			void setShaderStage(ShaderStage stage, const char* path);

			template<typename T>
			void setPushConstant(VkPipelineStageFlags stages = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_VERTEX_SHADER_BIT){
				setPushConstant(sizeof(T), stages);
			}
			
			void setPushConstant(size_t size, VkPipelineStageFlags stages);
			void setRenderPass(VkRenderPass renderPass);
			void setSubpass(uint32_t subpass);
			void pushSet(DescriptorSet* sets);

			void setVertexDescription(const PipelineVertexDescription &description);
			void setInstanceDescription(const PipelineVertexDescription &description);
			
		private:
			struct PipelineStage{
				bool required = false;
				std::filesystem::path path;
				ShaderStage stage;
			};

			struct PushConstant{
				std::size_t size = 0;
				VkPipelineStageFlags stages = 0;
				std::uint8_t binding = 0;
			};

			LogicalDevice* device = nullptr;
			PipelineStage shaderStages[SHADER_STAGE_COUNT];

			PipelineConfigInfo config;
			PushConstant pushConstant;
			std::vector<DescriptorSet*> sets{};

			bool hasVertexDescription = false;
			PipelineVertexDescription vertexDescription;
			bool hasInstanceDescription = false;
			PipelineVertexDescription instanceDescription;
	};
}