#pragma once

#include <vulkan/vulkan_core.h>

#include <optional>
#include <vector>

namespace NIX::Rendering
{
	class Surface;

	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		[[nodiscard]] bool IsComplete() const;
		
	};
	
	class PhysicalDevice
	{
	private:
		VkPhysicalDevice m_physicalDevice = nullptr;
		inline static const std::vector<const char*> m_deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
		
	public:
		PhysicalDevice() = default;
		PhysicalDevice(const PhysicalDevice& other) = delete;
		PhysicalDevice(PhysicalDevice&& other) noexcept = delete;
		PhysicalDevice& operator=(const PhysicalDevice& other) = delete;
		PhysicalDevice& operator=(PhysicalDevice&& other) noexcept = delete;
		~PhysicalDevice() = default;

		void Initialize(VkInstance instance, const Surface& surface);
		
		[[nodiscard]] const std::vector<const char*>& GetDeviceExtensions() const;
		[[nodiscard]] VkPhysicalDevice GetVkPhysicalDevice() const;
		
		[[nodiscard]] static SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice physicalDevice, const Surface& surface);
		[[nodiscard]] static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice physicalDevice, const Surface& surface);
		
	private:
		
		[[nodiscard]] bool IsDeviceSuitable(VkPhysicalDevice physicalDevice, const Surface& surface);
		[[nodiscard]] bool CheckDeviceExtensionSupport(VkPhysicalDevice physicalDevice);
	};
	
}
