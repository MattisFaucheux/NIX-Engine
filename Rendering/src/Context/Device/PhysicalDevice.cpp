#include "pch.h"

#include "Context/Device/PhysicalDevice.h"
#include "Context/Surface.h"

#include <set>
#include <stdexcept>

#include "Log/Logger.h"

namespace NIX::Rendering
{
	bool QueueFamilyIndices::IsComplete() const
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}

    void PhysicalDevice::Initialize(VkInstance instance, const Surface& surface)
	{
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        if (deviceCount == 0)
        {
            ASSERT(SOURCE_INFO, "Failed to find GPUs with Vulkan support !");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        for (const auto& physicalDevice : devices)
        {
            if (IsDeviceSuitable(physicalDevice, surface))
            {
                m_physicalDevice = physicalDevice;
                break;
            }
        }

        if (m_physicalDevice == nullptr)
        {
            ASSERT(SOURCE_INFO, "Failed to find a suitable GPU !");
        }
	}

	bool PhysicalDevice::IsDeviceSuitable(VkPhysicalDevice physicalDevice, const Surface& surface)
	{
		QueueFamilyIndices indices = FindQueueFamilies(physicalDevice, surface);

		bool extensionsSupported = CheckDeviceExtensionSupport(physicalDevice);

		bool swapChainAdequate = false;
		if (extensionsSupported)
		{
			SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(physicalDevice, surface);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}

		VkPhysicalDeviceFeatures supportedFeatures;
		vkGetPhysicalDeviceFeatures(physicalDevice, &supportedFeatures);

		return indices.IsComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
	}

	bool PhysicalDevice::CheckDeviceExtensionSupport(VkPhysicalDevice physicalDevice)
	{
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(m_deviceExtensions.begin(), m_deviceExtensions.end());

        for (const auto& extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
	}

	const std::vector<const char*>& PhysicalDevice::GetDeviceExtensions() const
	{
        return m_deviceExtensions;
	}

	VkPhysicalDevice PhysicalDevice::GetVkPhysicalDevice() const
	{
       return m_physicalDevice;
	}

	SwapChainSupportDetails PhysicalDevice::QuerySwapChainSupport(VkPhysicalDevice physicalDevice, const Surface& surface)
	{
        SwapChainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface.GetVkSurface(), &details.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface.GetVkSurface(), &formatCount, nullptr);

        if (formatCount != 0)
        {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface.GetVkSurface(), &formatCount, details.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface.GetVkSurface(), &presentModeCount, nullptr);

        if (presentModeCount != 0)
        {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface.GetVkSurface(), &presentModeCount, details.presentModes.data());
        }

        return details;
	}

	QueueFamilyIndices PhysicalDevice::FindQueueFamilies(VkPhysicalDevice physicalDevice, const Surface& surface)
	{
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilies)
        {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                indices.graphicsFamily = i;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface.GetVkSurface(), &presentSupport);

			if (presentSupport)
            {
                indices.presentFamily = i;
            }

            if (indices.IsComplete())
            {
                break;
            }

            i++;
        }
        return indices;
	}
}
