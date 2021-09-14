#include "pch.h"

#include "SynchronizationObjects.h"
#include "Context/Device/Device.h"
#include "SwapChain/SwapChain.h"

#include <stdexcept>

#include "Log/Logger.h"

namespace NIX::Rendering
{
	void SynchronizationObjects::Initialize()
	{
        m_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        m_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        m_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
        m_imagesInFlight.resize( MIN_IMAGE_COUNT, nullptr);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;


        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            if (vkCreateSemaphore(Device::GetSingleton()->GetVkLogicalDevice(), &semaphoreInfo, nullptr, &m_imageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(Device::GetSingleton()->GetVkLogicalDevice(), &semaphoreInfo, nullptr, &m_renderFinishedSemaphores[i]) != VK_SUCCESS
                || vkCreateFence(Device::GetSingleton()->GetVkLogicalDevice(), &fenceInfo, nullptr, &m_inFlightFences[i]) != VK_SUCCESS)
            {
				ASSERT(SOURCE_INFO, "Failed to create semaphores for a frame !");
            }
        }
	}

	void SynchronizationObjects::Cleanup()
	{
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			vkDestroySemaphore(Device::GetSingleton()->GetVkLogicalDevice(), m_renderFinishedSemaphores[i], nullptr);
			vkDestroySemaphore(Device::GetSingleton()->GetVkLogicalDevice(), m_imageAvailableSemaphores[i], nullptr);
			vkDestroyFence(Device::GetSingleton()->GetVkLogicalDevice(), m_inFlightFences[i], nullptr);
		}
	}

	const VkFence& SynchronizationObjects::GetInFlightFence(uint32_t imageIndex) const
	{
		return m_inFlightFences[imageIndex];
	}

	const VkSemaphore& SynchronizationObjects::GetImageAvailableSemaphore(uint32_t imageIndex) const
	{
		return m_imageAvailableSemaphores[imageIndex];
	}

	const VkFence& SynchronizationObjects::GetImageInFlight(uint32_t imageIndex) const
	{
		return m_imagesInFlight[imageIndex];
	}

	VkFence& SynchronizationObjects::MutateImageInFlight(uint32_t imageIndex)
	{
		return m_imagesInFlight[imageIndex];
	}

	const VkSemaphore& SynchronizationObjects::GetRenderFinishedSemaphore(uint32_t imageIndex) const
	{
		return m_renderFinishedSemaphores[imageIndex];
	}

	SynchronizationObjects::~SynchronizationObjects()
	{
		Cleanup();
	}
}
