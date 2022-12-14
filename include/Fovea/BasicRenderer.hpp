#pragma once

#include "builders/BasicRendererBuilder.hpp"
#include "CommandPool.hpp"
#include "utils.hpp"

#include <assert.h>

namespace Fovea::renderer{
	class BasicRenderer{
		public:
			BasicRenderer(BasicRendererBuilder& builder){
				initialize(builder);
			}

			~BasicRenderer(){
				vkDeviceWaitIdle(device->getDevice());
				swapChain.reset();

				vkFreeCommandBuffers(device->getDevice(), device->getCommandPool(FAMILY_GRAPHIC)->getCommandPool(), bufferCount, commandBuffers);
				delete[] commandBuffers;
			}

			void initialize(BasicRendererBuilder& builder){
				device = builder.device;
				extent = builder.extent;
				extent = builder.extent;
				refreshMode = builder.refreshMode;
				bufferCount = builder.bufferCount;
				clearColor = builder.clearColor;
				clearDepthStencil = builder.depthStencilClearValue;

				createSwapChain();
				createCommandBuffers();
			}

			void onWindowMinimized(){
				extent = {0, 0};
				windowResized = true;
			}

			void onWindowResied(VkExtent2D extent){
				this->extent = extent;

				// if the with or the height is null, do not render, otherwise render
				render = !(extent.width == 0 || extent.height == 0);
				windowResized = true;
			}

			void stopRendering(){
				render = false;
			}

			void continueRendering(){
				render = true;
			}
			
			// return the command buffer |!| VK_NULL_HANDLE if cannot render
			VkCommandBuffer beginFrame(){
				if (!render) return VK_NULL_HANDLE;

				VkResult result = swapChain->acquireNextImage(&currentBufferIndex);
				if (windowResized || result == VK_ERROR_OUT_OF_DATE_KHR){
					createSwapChain();
					return VK_NULL_HANDLE;
				}


				if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        			throw "failed to acquire swap chain image";
				}

				frameStarted = true;

				VkCommandBuffer commandBuffer = commandBuffers[currentBufferIndex];

				VkCommandBufferBeginInfo info{};
				info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				
				result = vkBeginCommandBuffer(commandBuffer, &info);
				if (result != VK_SUCCESS){
					throw (std::string("failed to begin command buffer | ") + resultToStr(result)).c_str();
				}

				return commandBuffer;
			}

			void endFrame(){
				assert(frameStarted ==  true && "cannot end a frame if it's not started");
				
				VkCommandBuffer commandBuffer = commandBuffers[currentBufferIndex];
				VkResult result = vkEndCommandBuffer(commandBuffer);

				if (result != VK_SUCCESS){
					throw (std::string("failed to end the command buffer | ") + resultToStr(result)).c_str();
				}
				
				result = swapChain->submitCommandBuffer(&commandBuffer, &currentBufferIndex);

				if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || windowResized){
					createSwapChain();
				} else if (result != VK_SUCCESS) {
					throw (std::string("failed to submit the command buffer | ") + resultToStr(result)).c_str();
				}

				frameStarted = false;
				currentBufferIndex = (currentBufferIndex + 1) % bufferCount;
			}

			void beginSwapChainRenderPass(){
				assert(frameStarted == true && "cannot begin swap chain render pass if a fram is not a started");

				VkCommandBuffer commandBuffer = commandBuffers[currentBufferIndex];

				VkRenderPassBeginInfo info{};
				info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				info.renderPass = swapChain->getRenderPass();
				info.framebuffer = swapChain->getFrameBuffer(currentBufferIndex);

				info.renderArea.offset = {0, 0};
				info.renderArea.extent = extent;

				VkClearValue clearValues[2];

				clearValues[0].depthStencil = clearDepthStencil;
				clearValues[1].color = clearColor;

				info.clearValueCount = 2;
				info.pClearValues = clearValues;

				vkCmdBeginRenderPass(commandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);

				VkViewport viewport{};
				viewport.x = 0.0f;
				viewport.y = 0.0f;
				viewport.width = static_cast<float>(extent.width);
				viewport.height = static_cast<float>(extent.height);
				viewport.minDepth = 0.0f;
				viewport.maxDepth = 1.0f;
				VkRect2D scissor{{0, 0}, extent};

				vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
				vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
			}

			void endSwapChainRenderPass(VkCommandBuffer commandBuffer){
    			vkCmdEndRenderPass(commandBuffer);
			}

			void setClearColor(VkClearColorValue color){
				clearColor = color;
			}

			void setClearDepthStencil(VkClearDepthStencilValue value){
				clearDepthStencil = value;
			}

			VkRenderPass getSwapChainRenderPass(){
				return swapChain->getRenderPass();
			}

		private:
			void createCommandBuffers(){
				commandBuffers = new VkCommandBuffer[bufferCount];

				VkCommandBufferAllocateInfo info{};
				info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				info.commandBufferCount = bufferCount;
				info.commandPool = device->getCommandPool(QueueFamily::FAMILY_GRAPHIC)->getCommandPool();
				info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				

				VkResult result = vkAllocateCommandBuffers(device->getDevice(), &info, commandBuffers);

				if (result != VK_SUCCESS){
					throw (std::string("failed to allocate command buffers | ") + resultToStr(result)).c_str();
				}
			}

			void createSwapChain(){
				vkDeviceWaitIdle(device->getDevice());

				SwapChainBuilder scBuilder;
				scBuilder.setDevice(device);
				scBuilder.setExtent(extent);
				scBuilder.setSwapchainBufferCount(bufferCount);
				scBuilder.setRefreshMode(refreshMode);

				if (swapChain == nullptr){
					swapChain = std::make_shared<SwapChain>(scBuilder);
				} else {
					oldSwapChain = swapChain;
					swapChain = std::make_shared<SwapChain>(oldSwapChain);

					swapChain->initialize(scBuilder);

					if (!swapChain->compareFormats(*oldSwapChain)){
						throw "swap chain image or depth format has changed";
					}
				}

				windowResized = false;
			}

			LogicalDevice* device = nullptr;
			std::shared_ptr<SwapChain> swapChain = nullptr;
			std::shared_ptr<SwapChain> oldSwapChain = nullptr;
			VkCommandBuffer* commandBuffers = nullptr;

			uint32_t bufferCount = 0;
			uint32_t currentBufferIndex = 0;

			bool frameStarted = false;
			bool render = true;
			bool windowResized = false;

			VkExtent2D extent;
			SwapChainRefreshMode refreshMode;
			VkClearColorValue clearColor;
			VkClearDepthStencilValue clearDepthStencil;
	};
}