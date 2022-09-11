#pragma once

#include "Framebuffer.hpp"
#include "LogicalDevice.hpp"
#include "../vulkan/vulkan.h"

#include <filesystem>

namespace Fovea{
	struct TextureBuilder{
		VkFilter magFilter = VK_FILTER_LINEAR;
		VkFilter minFilter = VK_FILTER_LINEAR;
		VkSamplerAddressMode addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
		VkSamplerAddressMode addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
		bool anisotropy = false;
		bool normalizeCoords = false;

		VkImageTiling tiling = VK_IMAGE_TILING_LINEAR;
		VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT;
	};

	class Texture{
		public:
			static constexpr uint32_t DEPTH_ATTACHMENT = -1;

			Texture(LogicalDevice* device, Framebuffer &framebuffer, uint32_t attachment, TextureBuilder& builder);
			Texture(LogicalDevice* device, const std::filesystem::path &path, TextureBuilder& builder);
			Texture(LogicalDevice* device, VkFormat format, VkExtent2D extent, void* data, uint32_t pixelSize, TextureBuilder& builder);

			~Texture();

			VkDescriptorImageInfo getDescriptorInfo();

			VkImage getImage();
			VkDeviceMemory getImageMemory();
			VkImageView getImageView();
			VkSampler getSampler();

		private:
			void createFromData(void* data, uint32_t pixelSize, TextureBuilder& builedr);

			void createImageSampler(TextureBuilder& builder);
			void createImage(TextureBuilder& builder);
			void createImageView(TextureBuilder& builder);

			template<typename T>
			struct Member{
				Member<T>(const T &other) {member = other;}
				Member<T>& operator=(const T &other) {member = other; return *this;}
				operator T&() {return member;}
				T& operator*() {return member;}
				T* operator->() {return &member;}
				T* operator&() {return &member;}

				T member;
				bool custom = false;
			};

			LogicalDevice* device = nullptr;
			VkExtent2D extent;
			VkFormat format;

			Member<VkImage> image = VK_NULL_HANDLE;
			Member<VkDeviceMemory> imageMemory = VK_NULL_HANDLE;
			Member<VkImageView> imageView = VK_NULL_HANDLE;
			VkSampler imageSampler = VK_NULL_HANDLE;

			VkFormat channelCountToVkFormat(int channel, VkImageTiling tiling, VkFormatFeatureFlags features);
			
	};
}

// Validation Error: [ UNASSIGNED-CoreValidation-Shader-InputNotProduced ] Object 0: handle = 0xa808d50000000033, type = VK_OBJECT_TYPE_SHADER_MODULE; | MessageID = 0x23e43bb7 | Vertex shader consumes input at location 2 but not provided