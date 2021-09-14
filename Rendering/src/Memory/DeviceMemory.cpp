#include "pch.h"

#include "Tools/Initializers.h"
#include "Context/Device/Device.h"

#include "Memory/DeviceMemory.h"
#include "Memory/Buffer.h"

#include <stdexcept>

#include "Log/Logger.h"

namespace NIX::Rendering
{
	DeviceMemory::~DeviceMemory()
	{
		vkDeviceWaitIdle(Device::GetSingleton()->GetVkLogicalDevice());
		Cleanup();
	}

	void DeviceMemory::Initialize(VkBuffer buffer, VkMemoryPropertyFlags properties)
	{
		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(Device::GetSingleton()->GetVkLogicalDevice(), buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo = Initializers::MemoryAllocateInfo();
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = Buffer::FindMemoryType(memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(Device::GetSingleton()->GetVkLogicalDevice(), &allocInfo, nullptr, &m_memory) != VK_SUCCESS)
		{
			ASSERT(SOURCE_INFO, "Failed to allocate buffer memory !");
		}

		vkBindBufferMemory(Device::GetSingleton()->GetVkLogicalDevice(), buffer, m_memory, 0);
	}

	void DeviceMemory::Initialize(VkImage image, VkMemoryPropertyFlags properties)
	{
		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(Device::GetSingleton()->GetVkLogicalDevice(), image, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = Buffer::FindMemoryType(memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(Device::GetSingleton()->GetVkLogicalDevice(), &allocInfo, nullptr, &m_memory) != VK_SUCCESS)
		{
			ASSERT(SOURCE_INFO, "Failed to allocate image memory!");
		}

		vkBindImageMemory(Device::GetSingleton()->GetVkLogicalDevice(), image, m_memory, 0);
	}

	void DeviceMemory::Cleanup()
	{
		vkFreeMemory(Device::GetSingleton()->GetVkLogicalDevice(), m_memory, nullptr);
	}

	void DeviceMemory::UpdateMemory(VkDeviceSize size, const void* data)
	{
		void* stagingData;
		vkMapMemory(Device::GetSingleton()->GetVkLogicalDevice(), m_memory, 0, size, 0, &stagingData);
		memcpy(stagingData, data, size);
		vkUnmapMemory(Device::GetSingleton()->GetVkLogicalDevice(), m_memory);
	}
}
