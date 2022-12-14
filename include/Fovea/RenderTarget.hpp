#pragma once
#include "builders/RenderTargetBuilder.hpp"
#include "Framebuffer.hpp"

#include "../vulkan/vulkan.h"

namespace Fovea{
	class RenderTarget{
		public:
			using ClearColor = RenderTargetBuilder::ClearColor;

			RenderTarget() = default;
			RenderTarget(RenderTargetBuilder &builder){
				initialize(builder);
			}

			~RenderTarget();

			RenderTarget(const RenderTarget &) = delete;
			RenderTarget& operator=(const RenderTarget &) = delete;

			void initialize(RenderTargetBuilder &builder);
			void beginRenderPass(VkCommandBuffer commandBuffer);
			void endRenderPass(VkCommandBuffer commandBuffer);
			VkRenderPass getRenderPass();
			Framebuffer* getFramebuffer();

			void resize(VkExtent2D extent);
		
		private:
			void createRenderPass(RenderTargetBuilder &builder);

			LogicalDevice* device = nullptr;
			std::vector<VkClearValue> clearValues;
			bool customFramebuffer = false;
			Framebuffer* framebuffer = nullptr;

			VkRenderPass renderPass = nullptr;
			VkViewport viewport;
			VkRect2D scissor;
	};
}