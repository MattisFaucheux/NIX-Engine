#pragma once
#include <vulkan/vulkan_core.h>

#include <vector>

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

const std::vector<const char*> VALIDATION_LAYERS = { "VK_LAYER_KHRONOS_validation" };

namespace NIX::Rendering
{
	class Debug
	{
	private:
		VkDebugUtilsMessengerEXT m_debugMessenger;
		VkInstance m_instance;
		
	public:
		Debug() = default;
		Debug(const Debug& other) = delete;
		Debug( Debug&& other) noexcept = delete;
		Debug& operator=(const Debug& other) = delete;
		Debug& operator=(Debug&& other) noexcept = delete;
		~Debug();
		
		void SetupDebugMessenger(VkInstance instance);
		
		[[nodiscard]] bool CheckValidationLayerSupport() const;
	private:

		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) const;
		
		[[nodiscard]] static VkResult CreateDebugUtilsMessengerExt(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

		static void DestroyDebugUtilsMessengerExt(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
															VkDebugUtilsMessageTypeFlagsEXT messageType,
															const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
															void* pUserData);
	};
}
