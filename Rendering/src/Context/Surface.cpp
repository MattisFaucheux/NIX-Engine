#include "pch.h"

#include "Context/Surface.h"

#include "Windowing/Window.h"
#include <stdexcept>

#include "Log/Logger.h"

namespace NIX::Rendering
{
	Surface::~Surface()
	{
		Cleanup();
	}

	void Surface::Initialize(Window& window, VkInstance instance)
	{
		m_instance = instance;
		
		if (window.InitSurface(m_instance, m_surface) != VK_SUCCESS)
		{
			ASSERT(SOURCE_INFO, "Failed to initialize the surface !");
		}
	}

	void Surface::Cleanup()
	{
		vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
	}

	VkSurfaceKHR Surface::GetVkSurface() const
	{
		return m_surface;
	}
}
