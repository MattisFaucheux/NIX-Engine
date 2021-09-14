#include "pch.h"

#include "Context/Device/LogicalDevice.h"
#include "Tools/Initializers.h"
#include "Context/Device/PhysicalDevice.h"
#include "Context/Surface.h"
#include "Debug/Debug.h"

#include <stdexcept>
#include <set>

#include "Log/Logger.h"

namespace NIX::Rendering
{
    void LogicalDevice::Initialize(const Surface& surface, const PhysicalDevice& physicalDevice)
	{
        QueueFamilyIndices indices = PhysicalDevice::FindQueueFamilies(physicalDevice.GetVkPhysicalDevice(), surface);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queueCreateInfo = Initializers::DeviceQueueCreateInfo();
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};
        deviceFeatures.samplerAnisotropy = VK_TRUE;

        VkDeviceCreateInfo createInfo = Initializers::DeviceCreateInfo();

        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();

        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = static_cast<uint32_t>(physicalDevice.GetDeviceExtensions().size());
        createInfo.ppEnabledExtensionNames = physicalDevice.GetDeviceExtensions().data();

        if (enableValidationLayers)
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
            createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
        }
        else
        {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(physicalDevice.GetVkPhysicalDevice() , &createInfo, nullptr, &m_device) != VK_SUCCESS)
        {
            ASSERT(SOURCE_INFO, "Failed to create logical device!");
        }

        vkGetDeviceQueue(m_device, indices.graphicsFamily.value(), 0, &m_graphicsQueue);
        vkGetDeviceQueue(m_device, indices.presentFamily.value(), 0, &m_presentQueue);
	}

	LogicalDevice::~LogicalDevice()
	{
        vkDestroyDevice(m_device, nullptr);
	}

	VkDevice LogicalDevice::GetVkDevice() const
	{
        return m_device;
	}

	VkQueue LogicalDevice::GetGraphicsQueue() const
	{
        return m_graphicsQueue;
	}

	VkQueue LogicalDevice::GetPresentQueue() const
	{
        return m_presentQueue;
	}
}
