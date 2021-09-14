#include "pch.h"

#include "Context/Device/Device.h"
#include "Context/Surface.h"

#include <cassert>
#include <stdexcept>

#include "Log/Logger.h"

namespace NIX::Rendering
{
	Device::Device()
	{
		if (m_singleton != nullptr)
		{
			assert(false);
			return;
		}

		m_singleton = this;
	}
	
	void Device::Initialize(VkInstance instance, const Surface& surface, Window& window)
	{
		m_physicalDevice.Initialize(instance, surface);

		m_logicalDevice.Initialize(surface, m_physicalDevice);

		m_window = &window;
	}

	bool Device::IsWindowResized() const
	{
		return m_window->GetIsFrameBufferResized();
	}

	void Device::SetIsWindowResized(bool value)
	{
		if (m_window != nullptr)
		{
			m_window->SetIsFrameBufferResized(value);
		}
	}

	void Device::WindowSize(int& width, int& height) const
	{
		const auto windowSize = m_window->GetFrameBufferSize();
		width = windowSize.first;
		height = windowSize.second;
	}

	Device* Device::GetSingleton()
	{
		if (m_singleton == nullptr)
		{
			ASSERT(SOURCE_INFO, "Device lost");
		}
		
		return m_singleton;
	}

	VkPhysicalDevice Device::GetVkPhysicalDevice() const
	{
		return m_physicalDevice.GetVkPhysicalDevice();
	}

	VkDevice Device::GetVkLogicalDevice() const
	{
		return m_logicalDevice.GetVkDevice();
	}

	Window* Device::GetWindow() const
	{
		return m_window;
	}

	VkQueue Device::GetGraphicsQueue() const
	{
		return m_logicalDevice.GetGraphicsQueue();
	}

	VkQueue Device::GetPresentQueue() const
	{
		return m_logicalDevice.GetPresentQueue();
	}

	std::pair<int, int> Device::GetFrameBufferSize() const
	{
		if (m_window == nullptr)
		{
			return { 0, 0 };
		}
		return m_window->GetFrameBufferSize();
	}

	VkFormat Device::FindDepthFormat()
	{
		return FindSupportedFormat(
			{	VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
						VK_IMAGE_TILING_OPTIMAL,
					VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
		);
	}

	VkFormat Device::FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling,VkFormatFeatureFlags features)
	{
		for (VkFormat format : candidates)
		{
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(m_physicalDevice.GetVkPhysicalDevice(), format, &props);

			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
			{
				return format;
			}
			if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
			{
				return format;
			}
		}

		ASSERT(SOURCE_INFO, "Failed to find supported format!");
		return VK_FORMAT_R8G8B8A8_SRGB;
	}
}


