#pragma once

#include "SwapChain.hpp"
#include "Buffer.hpp"
#include "../vulkan/vulkan.h"

#include <memory>
#include <vector>
#include <cassert>
#include <queue>

namespace Fovea{
	class Renderer{
		public:
			~Renderer();

			void initialize(uint32_t vertexBufferSize);

			VkRenderPass getSwapChainRenderPass() const {return swapChain->getRenderPass();}

			bool isFrameInProgress() const {return isFrameStarted;}

			VkCommandBuffer getCurrentCommandBuffer() const {
				assert(isFrameStarted && "Cannot get command buffer whene frame not in progress");
				return commandBuffers[currentFrameIndex];
			};

			int getFrameIndex() const {
				assert(isFrameStarted && "Cannot get frame index whene frame not in progress");
				return currentFrameIndex;
			}

			enum class Topology{
				Quad = 0,
				Trigone = 1,
				Line = 1
			};

			SwapChain &getSwapChain() noexcept {return *swapChain;}

			VkCommandBuffer beginFrame();

			void endFrame();

			void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);

			void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

			void setClearColor(float r = 0.f, float g = 0.f, float b = 0.f, float a = 0.f);

			void setViewPortPosition(const float &x = 0.f, const float &y = 0.f) {viewport.x = x; viewport.y = y;}

			void setViewPortSize(const float &width, const float &height) {viewport.width = width; viewport.height = height;}

			void windowResized(uint32_t width, uint32_t height);

			void windowMinimized();

			void set(void* v, uint32_t vertexCount);

			void setVertexSize(uint32_t size, size_t minOffsetAlignement);

			uint32_t getVertexSize();

			void render();

			void setData(uint32_t offset, uint32_t count, void* data);

			void setTopology(Topology typology);

			void renderQuad(void *v0, void *v1, void *v2, void *v3);

			void renderTrigone(void *v0, void *v1, void *v2);

			void renderLine(void *v0, void *v1);

			void renderPoint(void *v0);

			Topology getTopology();

			int getCurrentFrameIndex();

			VkClearColorValue getClearColor();

		private:
			void createCommandBuffers();
			void freeCommandBuffers();
			void recreateSwapChain();
			void createVertexBuffer(uint32_t count);
			void createIndexBuffer(uint32_t count);

			void flush();
			void reset();

			std::unique_ptr<SwapChain> swapChain = nullptr;
			std::vector<VkCommandBuffer> commandBuffers;

			uint32_t currentImageIndex = 0;
			int currentFrameIndex = 0;
			bool isFrameStarted = false;
			bool isWindowResized = false;
			bool isWindowMinimized = false;

			VkClearColorValue clearColor = {0.f, 0.f, 0.f, 0.f};
			VkViewport viewport;
			VkRect2D scissor;
			VkExtent2D windowExtent = {1080, 720};

			Buffer vertexBuffer;
			Buffer indexBuffer;

			uint32_t vertexSize = 10;
			uint32_t alignement = 10;
			uint32_t vertexBufferUsedSize = 0;
			uint32_t indexUsed = 0;
			uint32_t maxVertexSize = 0;
			Topology topology = Topology::Quad;
	};
}