#include "pch.h"
#include "Windowing/Window.h"

#include <stb_image.h>

#include "Log/Logger.h"

namespace NIX::Rendering
{

	Window::Window(const char* name, const int width, const int height, const char* iconPath)
	{
		InitWindow(name, width, height, iconPath);
	}

	void Window::InitWindow(const char* name, const int width, const int height, const char* iconPath)
	{
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

		ms_width = width;
		ms_height = height;

		m_window = glfwCreateWindow(ms_width, ms_height, name, nullptr, nullptr);
		glfwSetWindowUserPointer(m_window, this);
		glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);

		SetWindowIcon(iconPath);
		
		m_isRunning = true;
	}

	Window::~Window()
	{
		Cleanup();
	}

	bool Window::InitGlfw()
	{
		return glfwInit() == GLFW_TRUE;
	}

	float Window::GetTimeGlfw()
	{
		return (float)glfwGetTime();
	}

	void Window::Cleanup()
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	void Window::SetWindowIcon(const char* iconPath)
	{
		if (iconPath == nullptr)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "Icon path is null");
			return;
		}
		
		GLFWimage icon;
		icon.pixels = stbi_load(iconPath, &icon.width, &icon.height, 0, 4);

		if (icon.pixels == nullptr)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "Failed to load icon %s for the window", iconPath);
			return;
		}
		
		glfwSetWindowIcon(m_window, 1, &icon);
		stbi_image_free(icon.pixels);
	}


	void Window::ReceiveInput()
	{
		if (glfwWindowShouldClose(m_window))
		{
			m_isRunning = false;
		}

		glfwPollEvents();
	}

	void Window::framebufferResizeCallback(GLFWwindow* /*window*/, int width, int height)
	{
		ms_width = width;
		ms_height = height;
		ms_isFrameBufferResized = true;
	}

	VkResult Window::InitSurface(const VkInstance& vkInstance, VkSurfaceKHR& vkSurface)
	{
		return glfwCreateWindowSurface(vkInstance, m_window, nullptr, &vkSurface);

	}

	int Window::GetWindowWidth() const
	{
		return ms_width;
	}

	int Window::GetWindowHeight() const
	{
		return ms_height;
	}

	std::pair<int, int> Window::GetFrameBufferSize() const
	{
		if (m_window == nullptr)
		{
			return {0, 0};
		}
		
		std::pair<int, int> frameBufferSize;
		glfwGetFramebufferSize(m_window, &frameBufferSize.first, &frameBufferSize.second);
		return frameBufferSize;
	}

	bool Window::GetIsFrameBufferResized() const
	{
		return ms_isFrameBufferResized;
	}

	void Window::SetIsFrameBufferResized(const bool isChanged)
	{
		ms_isFrameBufferResized = isChanged;
	}

	bool Window::GetIsRunning() const
	{
		return m_isRunning;
	}

	GLFWwindow* Window::GetWindow()
	{
		return m_window;
	}

	void Window::SetName(const char* name)
	{
		glfwSetWindowTitle(m_window, name);
	}

	void Window::SetCursorPosition(double xPos, double yPos)
	{
		glfwSetCursorPos(m_window, xPos, yPos);
	}

	std::vector<const char*> Window::GetRequiredExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		return extensions;
	}
}


