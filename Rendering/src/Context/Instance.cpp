#include "pch.h"

#include "Context/Instance.h"
#include "Tools/Initializers.h"
#include "Debug/Debug.h"

#include <vector>
#include <stdexcept>


#include "Log/Logger.h"
#include "Windowing/EInputKeyboard.h"

namespace NIX::Rendering
{
	Instance::~Instance()
	{
		vkDestroyInstance(m_instance, nullptr);
	}

	void Instance::Initialize(const VkApplicationInfo& appInfo, std::vector<const char*>& requiredExtension)
	{
        VkInstanceCreateInfo createInfo = Initializers::InstanceCreateInfo(appInfo);

        if (enableValidationLayers)
        {
            requiredExtension.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
            createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
        }
        else
        {
            createInfo.enabledLayerCount = 0;
        }

        createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtension.size());
        createInfo.ppEnabledExtensionNames = requiredExtension.data();
		
        if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS)
        {
            ASSERT(SOURCE_INFO, "Failed to create vulkan instance");
        }
	}

	VkInstance Instance::GetVkInstance() const
	{
		return m_instance;
	}
}
