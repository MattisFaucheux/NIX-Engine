#include "pch.h"

#include "Tools/Initializers.h"
#include "Context/Device/Device.h"
#include "CommandPool.h"

#include "Memory/Buffer.h"
#include "Memory/EBufferType.h"
#include "Memory/DeviceMemory.h"

#include <stdexcept>

#include "Log/Logger.h"

namespace NIX::Rendering
{
    Buffer::~Buffer()
    {
        vkDeviceWaitIdle(Device::GetSingleton()->GetVkLogicalDevice());
    	
    	vkDestroyBuffer(Device::GetSingleton()->GetVkLogicalDevice(), m_buffer, nullptr);        
    }

    void Buffer::Initialize(const CommandPool& commandPool, uint64_t bufferSize, void* data, EBufferType bufferType)
    {
        m_bufferSize = bufferSize;
        
        switch (bufferType)
        {
        case EBufferType::VERTEX:
            CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
            break;
        case EBufferType::INDEX:
            CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
            break;
        case EBufferType::UNIFORM:
            CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
            return;
        default:
            ASSERT(SOURCE_INFO, "Buffer type don't exist !");
        }
    	
        Buffer stagingBuffer;
    	
        stagingBuffer.CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        stagingBuffer.Update(bufferSize, data);

        CopyBuffer(commandPool, stagingBuffer, bufferSize);
        
    }

    void Buffer::Cleanup()
    {
        vkDeviceWaitIdle(Device::GetSingleton()->GetVkLogicalDevice());
    	
        m_memory.Cleanup();
        vkDestroyBuffer(Device::GetSingleton()->GetVkLogicalDevice(), m_buffer, nullptr);
    }

    void Buffer::Update(VkDeviceSize size, const void* data)
    {
        m_memory.UpdateMemory(size, data);
    }

    void Buffer::InitDescriptorBufferInfo(VkDescriptorBufferInfo& descriptorBufferInfo) const
    {
        descriptorBufferInfo.buffer = m_buffer;
        descriptorBufferInfo.offset = 0;
        descriptorBufferInfo.range = m_bufferSize;
    }

    void Buffer::BindVertexBuffer(VkCommandBuffer commandBuffer) const
    {
        VkBuffer vertexBuffers[] = { m_buffer };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    }

    void Buffer::BindIndexBuffer(VkCommandBuffer commandBuffer) const
    {
        vkCmdBindIndexBuffer(commandBuffer, m_buffer, 0, VK_INDEX_TYPE_UINT32);
    }

    void Buffer::CopyBufferToImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout,
	    int regionCount, VkBufferImageCopy& region) const
    {
        vkCmdCopyBufferToImage(
            commandBuffer,
            m_buffer,
            image,
            imageLayout,
            regionCount,
            &region
        );
    }

    void Buffer::CreateBuffer(uint64_t size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
    {
        VkBufferCreateInfo bufferInfo = Initializers::BufferCreateInfo(size, usage, VK_SHARING_MODE_EXCLUSIVE);

        if (vkCreateBuffer(Device::GetSingleton()->GetVkLogicalDevice(), &bufferInfo, nullptr, &m_buffer) != VK_SUCCESS)
        {
            ASSERT(SOURCE_INFO, "Failed to create buffer !");
        }

        m_memory.Initialize(m_buffer, properties);
    }

    void Buffer::CopyBuffer(const CommandPool& commandPool, Buffer& srcBuffer, VkDeviceSize size) const
    {
        VkCommandBuffer commandBuffer = BeginSingleTimeCommands(commandPool);

        VkBufferCopy copyRegion{};
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, srcBuffer.m_buffer, m_buffer, 1, &copyRegion);

        EndSingleTimeCommands(commandPool, commandBuffer);
    }

    VkCommandBuffer Buffer::BeginSingleTimeCommands(const CommandPool& commandPool)
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = commandPool.GetCommandPool();
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(Device::GetSingleton()->GetVkLogicalDevice(), &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        return commandBuffer;
    }

    void Buffer::EndSingleTimeCommands(const CommandPool& commandPool, VkCommandBuffer commandBuffer)
    {
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(Device::GetSingleton()->GetGraphicsQueue(), 1, &submitInfo, nullptr);
        vkQueueWaitIdle(Device::GetSingleton()->GetGraphicsQueue());

        vkFreeCommandBuffers(Device::GetSingleton()->GetVkLogicalDevice(), commandPool.GetCommandPool(), 1, &commandBuffer);
    }

    uint32_t Buffer::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(Device::GetSingleton()->GetVkPhysicalDevice(), &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
        {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
            {
                return i;
            }
        }

        ASSERT(SOURCE_INFO, "Failed to find suitable memory type !");
        return 0;
    }
}


