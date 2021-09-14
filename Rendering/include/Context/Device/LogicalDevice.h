#pragma once
#include <vulkan/vulkan_core.h>

namespace NIX::Rendering
{
	class Surface;
	class PhysicalDevice;

	class LogicalDevice
	{
	private:
		VkDevice m_device = nullptr;
		VkQueue m_graphicsQueue = nullptr;
		VkQueue m_presentQueue = nullptr;
		
	public:
		LogicalDevice() = default;
		LogicalDevice(const LogicalDevice& other) = delete;
		LogicalDevice(LogicalDevice&& other) noexcept = delete;
		LogicalDevice& operator=(const LogicalDevice& other) = delete;
		LogicalDevice& operator=(LogicalDevice&& other) noexcept = delete;
		~LogicalDevice();

		void Initialize(const Surface& surface, const PhysicalDevice& physicalDevice);

		[[nodiscard]] VkDevice GetVkDevice() const;
		[[nodiscard]] VkQueue GetGraphicsQueue() const;
		[[nodiscard]] VkQueue GetPresentQueue() const;
	
	};
}
