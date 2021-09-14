#include "pch.h"

#include "SwapChain/SwapChain.h"
#include "Tools/Initializers.h"
#include "Context/Device/PhysicalDevice.h"
#include "RenderPass/RenderPass.h"
#include "Context/Surface.h"
#include "Context/Device/Device.h"
#include "Material/Texture.h"

#include <stdexcept>
#include <algorithm>
#include <array>

#include "Log/Logger.h"

namespace NIX::Rendering
{
	SwapChain::~SwapChain()
	{
        for (auto& swapChainFrameBuffer : m_swapChainFrameBuffers)
        {
            vkDestroyFramebuffer(Device::GetSingleton()->GetVkLogicalDevice(), swapChainFrameBuffer, nullptr);
        }
        		
        vkDestroySwapchainKHR(Device::GetSingleton()->GetVkLogicalDevice(), m_swapChain, nullptr);
	}

    void SwapChain::Initialize(const Surface& surface)
    {
		const SwapChainSupportDetails swapChainSupport = PhysicalDevice::QuerySwapChainSupport(Device::GetSingleton()->GetVkPhysicalDevice(), surface);

        const VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
        const VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
        const VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

        if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
        {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo = Initializers::SwapChainCreateInfoKHR(surface.GetVkSurface());
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = PhysicalDevice::FindQueueFamilies(Device::GetSingleton()->GetVkPhysicalDevice(), surface);
        uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

        if (indices.graphicsFamily != indices.presentFamily)
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;

        createInfo.oldSwapchain = nullptr;

        if (vkCreateSwapchainKHR(Device::GetSingleton()->GetVkLogicalDevice(), &createInfo, nullptr, &m_swapChain) != VK_SUCCESS)
        {
            ASSERT(SOURCE_INFO, "Failed to create swap chain !");
        }

        vkGetSwapchainImagesKHR(Device::GetSingleton()->GetVkLogicalDevice(), m_swapChain, &imageCount, nullptr);
        m_swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(Device::GetSingleton()->GetVkLogicalDevice(), m_swapChain, &imageCount, m_swapChainImages.data());

        m_swapChainImageFormat = surfaceFormat.format;
        m_swapChainExtent = extent;

        CreateImageViews();
    }

	VkSwapchainKHR SwapChain::GetSwapChain() const
	{
        return m_swapChain;
	}

	const VkFormat& SwapChain::GetImageFormat() const
	{
        return m_swapChainImageFormat;
	}

	const VkExtent2D& SwapChain::GetExtent() const
	{
        return m_swapChainExtent;
	}

	const std::vector<VkFramebuffer>& SwapChain::GetFrameBuffers() const
	{
        return m_swapChainFrameBuffers;
	}

	const std::vector<VkImage>& SwapChain::GetSwapChainImages() const
	{
        return m_swapChainImages;
	}

	void SwapChain::CreateDepthImageView()
	{
        m_depthImage.Initialize(*this);
        m_depthImageView.Initialize(m_depthImage.GetVkImage(), Device::GetSingleton()->FindDepthFormat(), VK_IMAGE_ASPECT_DEPTH_BIT);
	}

	void SwapChain::CreateFrameBuffers(const RenderPass& renderPass)
	{
        CreateDepthImageView();
		
		m_swapChainFrameBuffers.resize(m_imageViews.size());

        for (size_t i = 0; i < m_imageViews.size(); i++)
        {
            std::array<VkImageView, 2> attachments = {
                m_imageViews[i].GetVkImageView(),
                m_depthImageView.GetVkImageView()
            };

            VkFramebufferCreateInfo frameBufferInfo = Initializers::FrameBufferCreateInfo();
            frameBufferInfo.renderPass = renderPass.GetVkRenderPass();
            frameBufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            frameBufferInfo.pAttachments = attachments.data();
            frameBufferInfo.width = m_swapChainExtent.width;
            frameBufferInfo.height = m_swapChainExtent.height;
            frameBufferInfo.layers = 1;

            if (vkCreateFramebuffer(Device::GetSingleton()->GetVkLogicalDevice(), &frameBufferInfo, nullptr, &m_swapChainFrameBuffers[i]) != VK_SUCCESS)
            {
                ASSERT(SOURCE_INFO, "Failed to create frame-buffer !");
            }
        }
	}

	void SwapChain::Cleanup()
	{
        const VkDevice device = Device::GetSingleton()->GetVkLogicalDevice();

        m_depthImageView.Cleanup();
        m_depthImage.Cleanup();

        for (auto* frameBuffer : m_swapChainFrameBuffers)
        {
            vkDestroyFramebuffer(device, frameBuffer, nullptr);
        }
	}

	void SwapChain::CleanupImageViews()
	{
        for (auto& image : m_imageViews)
        {
            image.Cleanup();
        }
	}

	void SwapChain::Destroy()
	{
        vkDestroySwapchainKHR(Device::GetSingleton()->GetVkLogicalDevice(), m_swapChain, nullptr);
	}


	void SwapChain::CreateImageViews()
    {
        m_imageViews.resize(m_swapChainImages.size());

        for (uint32_t i = 0; i < m_swapChainImages.size(); i++)
        {
            m_imageViews[i].Initialize(m_swapChainImages[i], m_swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
        }
    }
	
	VkSurfaceFormatKHR SwapChain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
	{
        for (const auto& availableFormat : availableFormats)
        {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return availableFormat;
            }
        }

        // if formats asked not existing we take the first in list
        return availableFormats[0];
	}

	VkPresentModeKHR SwapChain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
	{
        for (const auto& availablePresentMode : availablePresentModes)
        {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D SwapChain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
	{
        if (capabilities.currentExtent.width != UINT32_MAX)
        {
            return capabilities.currentExtent;
        }
        else
        {
            std::pair<int,int> frameBufferSize =  Device::GetSingleton()->GetFrameBufferSize();
        	
            VkExtent2D actualExtent = { static_cast<uint32_t>(frameBufferSize.first), static_cast<uint32_t>(frameBufferSize.second) };
        	
            actualExtent.width = (std::max)(capabilities.minImageExtent.width, (std::min)(capabilities.maxImageExtent.width, actualExtent.width));
            actualExtent.height = (std::max)(capabilities.minImageExtent.height, (std::min)(capabilities.maxImageExtent.height, actualExtent.height));

            return actualExtent;
        }
	}

}


