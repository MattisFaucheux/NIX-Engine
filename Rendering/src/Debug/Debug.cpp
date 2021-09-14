#include "pch.h"

#include "Debug/Debug.h"


#include <iostream>
#include <stdexcept>
#include <vector>

#include "Log/Logger.h"

namespace NIX::Rendering
{
	Debug::~Debug()
	{
		if constexpr (enableValidationLayers)
		{
			DestroyDebugUtilsMessengerExt(m_instance, m_debugMessenger, nullptr);
		}
	}

	void Debug::SetupDebugMessenger(VkInstance instance)
	{
		if constexpr (!enableValidationLayers)
		{
			return;
		}
		else
		{
			if (!CheckValidationLayerSupport())
			{
				ASSERT(SOURCE_INFO, "Validation layers requested, but not available !");
			}

			m_instance = instance;

			VkDebugUtilsMessengerCreateInfoEXT createInfo;


			PopulateDebugMessengerCreateInfo(createInfo);

			if (CreateDebugUtilsMessengerExt(m_instance, &createInfo, nullptr, &m_debugMessenger) != VK_SUCCESS)
			{
				ASSERT(SOURCE_INFO, "Failed to set up debug messenger !");
			}
		}
	}

	void Debug::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) const
	{
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = DebugCallback;
	}
	
	bool Debug::CheckValidationLayerSupport() const
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : VALIDATION_LAYERS) 
		{
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers)
			{
				if (strcmp(layerName, layerProperties.layerName) == 0)
				{
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				return false;
			}
		}

		return true;
	}

	VkResult Debug::CreateDebugUtilsMessengerExt(VkInstance instance,
		const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator,
		VkDebugUtilsMessengerEXT* pDebugMessenger)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else
		{
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void Debug::DestroyDebugUtilsMessengerExt(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
		const VkAllocationCallbacks* pAllocator)
	{
		const auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			func(instance, debugMessenger, pAllocator);
		}
	}

	VkBool32 Debug::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT /*messageType*/, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* /*pUserData*/)
	{
		if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		{
			std::cerr << "Validation layer: " << pCallbackData->pMessage << "\n" << std::endl;
		}
		
		return VK_FALSE;
	}
}
