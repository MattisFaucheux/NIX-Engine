#pragma once

#include "LogicalDevice.h"
#include "PhysicalDevice.h"
#include "Windowing/Window.h"

namespace NIX::Rendering
{
	class Device
	{
	private:
		inline static Device* m_singleton = nullptr;
		PhysicalDevice m_physicalDevice;
		LogicalDevice m_logicalDevice;
		Window* m_window = nullptr;
		
	public:
		Device();
		Device(const Device& other) = delete;
		Device(Device&& other) noexcept = delete;
		Device& operator=(const Device& other) = delete;
		Device& operator=(Device&& other) noexcept = delete;
		~Device() = default;
		
		void Initialize(VkInstance instance, const Surface& surface, Window& window);

		[[nodiscard]] bool IsWindowResized() const;
		void SetIsWindowResized(bool value);
		void WindowSize(int& width, int& height) const;
		
		
		[[nodiscard]] static Device* GetSingleton();

		[[nodiscard]] VkPhysicalDevice GetVkPhysicalDevice() const;
		[[nodiscard]] VkDevice GetVkLogicalDevice() const;
		[[nodiscard]] Window* GetWindow() const;

		[[nodiscard]] VkQueue GetGraphicsQueue() const;
		[[nodiscard]] VkQueue GetPresentQueue() const;
		
		[[nodiscard]] std::pair<int, int> GetFrameBufferSize() const;
		
		[[nodiscard]] VkFormat FindDepthFormat();
	private:

		[[nodiscard]] VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	};
}
