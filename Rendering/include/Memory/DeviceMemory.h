#pragma once
#include <vulkan/vulkan_core.h>

namespace NIX::Rendering
{
	class DeviceMemory
	{
	private:
		VkDeviceMemory m_memory;
	public:
		DeviceMemory() = default;
		DeviceMemory(const DeviceMemory& other) = default;
		DeviceMemory(DeviceMemory&& other) noexcept = delete;
		DeviceMemory& operator=(const DeviceMemory& other) = delete;
		DeviceMemory& operator=(DeviceMemory&& other) noexcept = delete;
		~DeviceMemory();

		void Initialize(VkBuffer buffer, VkMemoryPropertyFlags properties);
		void Initialize(VkImage image, VkMemoryPropertyFlags properties);
		void Cleanup();
		void UpdateMemory(VkDeviceSize size, const void* data);
	};
}

