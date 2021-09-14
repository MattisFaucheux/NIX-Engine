#pragma once

#include <vulkan/vulkan_core.h>

namespace NIX::Rendering
{
	class Window;
	
	class Surface
	{
	private:
		VkSurfaceKHR m_surface = nullptr;
		VkInstance m_instance = nullptr;
	public:
		Surface() = default;
		Surface(const Surface& other) = delete;
		Surface(Surface&& other) noexcept = delete;
		Surface& operator=(const Surface& other) = delete;
		Surface& operator=(Surface&& other) noexcept = delete;
		~Surface();

		void Initialize(Window& window, VkInstance instance);
		void Cleanup();
		
		[[nodiscard]] VkSurfaceKHR GetVkSurface() const;
	};

}
