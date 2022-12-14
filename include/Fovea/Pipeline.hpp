#pragma once

#include "builders/PipelineBuilder.hpp"
#include "DescriptorSet.hpp"

namespace Fovea{
	class Pipeline{
		friend class PipelineLibrary;
		public:
			Pipeline() = default;
			Pipeline(PipelineBuilder& builder);
			~Pipeline();

			Pipeline(const Pipeline &) = delete;
			Pipeline& operator=(const Pipeline &) = delete;

			void initialize(PipelineBuilder& builder);
			const PipelineConfigInfo& getConfig();
			void bind(VkCommandBuffer commandBuffer, uint32_t setsIndex[]);
			void bindPushConstant(VkCommandBuffer, void* data); 

		private:
			struct ShaderModule{
				VkShaderModule shaderModule;
				VkShaderStageFlagBits stage;
			};

			void createGraphicPipeline(PipelineBuilder &builder);
			void createPipelineLayout(PipelineBuilder &builder);
			void createShaderModules(PipelineBuilder &builder);
			void createShaderModule(const std::vector<char>& code, VkShaderModule& shaderModule);

			LogicalDevice* device = nullptr;

			VkPipeline pipeline = VK_NULL_HANDLE;
			VkPipelineLayout layout = VK_NULL_HANDLE;
			uint32_t pushConstantSize = 0;
			VkShaderStageFlags pushConstantStages;

			PipelineConfigInfo config;
			std::vector<ShaderModule> shaderModules;
			std::vector<DescriptorSet*> sets;
	};
}