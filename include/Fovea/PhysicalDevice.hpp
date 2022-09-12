#pragma once

#include "builders/PhysicalDeviceBuilder.hpp"
#include "Instance.hpp"
#include "../vulkan/vulkan.h"

#include <array>

namespace Fovea{
	class PhysicalDevice{
		public:
			struct SwapChainSupport{
				VkSurfaceCapabilitiesKHR capabilities{};
				std::vector<VkSurfaceFormatKHR> formats{};
				std::vector<VkPresentModeKHR> presentModes{};
			};

			struct FamilyDetails{
				uint32_t family = 0;
				QueueFamily type = QueueFamily::FAMILY_NONE; // used as a non intialized value
			};

			PhysicalDevice() = default;
			PhysicalDevice(PhysicalDeviceBuidler &builder){
				initialize(builder);
			}
			~PhysicalDevice();

			PhysicalDevice(const PhysicalDevice &) = delete;
			PhysicalDevice& operator=(const PhysicalDevice &) = delete;

			void initialize(PhysicalDeviceBuidler &builder);

			VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
			uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

			FamilyDetails getFamily(QueueFamily family) const;
			uint32_t getPresentFamily();

			std::vector<FamilyDetails> getFamilies() const {return families;}
			const std::bitset<FAMILY_COUNT>& getEnabledFamilies();
			VkPhysicalDeviceProperties& getProperties(); 
			VkPhysicalDeviceFeatures getEnabledFeatures();
			VkPhysicalDevice getDevice();
			SwapChainSupport getSwapChainSupport();
			Instance* getInstance();			

		private:
			void checkDeviceExtensionSupport(VkPhysicalDevice device, PhysicalDeviceBuidler &builder);
			bool isSuitableDevice(VkPhysicalDevice device, PhysicalDeviceBuidler &builder);
			std::array<uint32_t, FAMILY_COUNT> getFamilies(VkPhysicalDevice physicalDevice, PhysicalDeviceBuidler &builder);
			bool checkDeviceExtensions(VkPhysicalDevice device, PhysicalDeviceBuidler &builder);
			SwapChainSupport getSwapChainSupport(VkPhysicalDevice device);
			bool checkFeatures(VkPhysicalDeviceFeatures features, PhysicalDeviceBuidler &builder);

			std::vector<FamilyDetails> families;
			std::bitset<FAMILY_COUNT> enabledFamiles;
			uint32_t presentFamily = -1;

			VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
			VkPhysicalDeviceProperties properties{};
			VkPhysicalDeviceFeatures features{};
			SwapChainSupport swapChainSupport{};
			Instance* instance = nullptr;
	};
}