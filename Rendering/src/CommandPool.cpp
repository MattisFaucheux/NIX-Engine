#include "pch.h"

#include "CommandPool.h"
#include "Tools/Initializers.h"
#include "Context/Device/Device.h"
#include "Context/Surface.h"

#include <stdexcept>

#include "Log/Logger.h"

namespace NIX::Rendering
{
	CommandPool::~CommandPool()
	{
		Cleanup();
	}
	
	void CommandPool::Initialize(const Surface& surface)
	{
		const QueueFamilyIndices queueFamilyIndices = PhysicalDevice::FindQueueFamilies(Device::GetSingleton()->GetVkPhysicalDevice(), surface);

		VkCommandPoolCreateInfo poolInfo = Initializers::CommandPoolCreateInfo();
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		if (vkCreateCommandPool(Device::GetSingleton()->GetVkLogicalDevice(), &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS)
		{
			ASSERT(SOURCE_INFO, "Failed to create command pool !");
		}
	}

	void CommandPool::Cleanup()
	{
		vkDestroyCommandPool(Device::GetSingleton()->GetVkLogicalDevice(), m_commandPool, nullptr);
	}

	VkCommandPool CommandPool::GetCommandPool() const
	{
		return m_commandPool;
	}
}
