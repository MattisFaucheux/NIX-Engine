#pragma once

#include "API/DLLExportRendering.h"

#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan_core.h>

#include <vector>

#include "GLFW/glfw3.h"


namespace NIX::Rendering
{
	class Window
	{
	public:

		Window() = delete;
		RENDERING_LIBRARY_API Window(const char* name, const int width = 800, const int height = 600, const char* iconPath = nullptr);

		Window(const Window& other) = delete;
		Window(Window&& other) noexcept = delete;
		Window& operator=(const Window& other) = delete;
		Window& operator=(Window&& other) noexcept = delete;

		RENDERING_LIBRARY_API ~Window();


		RENDERING_LIBRARY_API static bool InitGlfw();
		RENDERING_LIBRARY_API static float GetTimeGlfw();
		
		RENDERING_LIBRARY_API void ReceiveInput();

		/* Init Vulkan surface*/
		RENDERING_LIBRARY_API VkResult InitSurface(const VkInstance& vkInstance, VkSurfaceKHR& vkSurface);

		RENDERING_LIBRARY_API [[nodiscard]] int GetWindowWidth() const;
		RENDERING_LIBRARY_API [[nodiscard]] int GetWindowHeight() const;
		RENDERING_LIBRARY_API [[nodiscard]] std::pair<int, int> GetFrameBufferSize() const;
		RENDERING_LIBRARY_API [[nodiscard]] bool GetIsFrameBufferResized() const;
		RENDERING_LIBRARY_API void SetIsFrameBufferResized(const bool isChanged);
		
		RENDERING_LIBRARY_API [[nodiscard]] bool GetIsRunning() const;
		RENDERING_LIBRARY_API [[nodiscard]] std::vector<const char*> GetRequiredExtensions();

		RENDERING_LIBRARY_API GLFWwindow* GetWindow();
		RENDERING_LIBRARY_API void SetName(const char* name);
		
		RENDERING_LIBRARY_API void SetCursorPosition(double xPos, double yPos);

	private:

		void InitWindow(const char* name, const int width, const int height, const char* iconPath);
		void Cleanup();

		void SetWindowIcon(const char* iconPath);
		
		static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

		GLFWwindow* m_window;

		bool m_isRunning = false;;
		
		inline static int ms_width = 0;
		inline static int ms_height = 0;
		inline static bool ms_isFrameBufferResized = false;

		inline static std::pair<double, double> m_mousePos;
	};
	
}
