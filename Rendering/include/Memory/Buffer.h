#pragma once
#include <vulkan/vulkan_core.h>

#include "Memory/DeviceMemory.h"

namespace NIX::Rendering
{
	enum class EBufferType;
	class CommandPool;

	class Buffer
	{
	private:
		VkBuffer m_buffer;
		DeviceMemory m_memory;
		VkDeviceSize m_bufferSize;
		EBufferType m_bufferType;
	
	public:
		Buffer() = default;
		Buffer(const Buffer& other) = default;
		Buffer(Buffer&& other) noexcept = delete;
		Buffer& operator=(const Buffer& other) = delete;
		Buffer& operator=(Buffer&& other) noexcept = delete;
		~Buffer();

		void Initialize(const CommandPool& commandPool, uint64_t bufferSize, void* data, EBufferType bufferType);
		void CreateBuffer(uint64_t size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
		void InitDescriptorBufferInfo(VkDescriptorBufferInfo& descriptorBufferInfo) const;
		void Update(VkDeviceSize size, const void* data);
		void Cleanup();

		void BindVertexBuffer(VkCommandBuffer commandBuffer) const;
		void BindIndexBuffer(VkCommandBuffer commandBuffer) const;
		void CopyBufferToImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, int regionCount, VkBufferImageCopy& region) const;
		
		static uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		static VkCommandBuffer BeginSingleTimeCommands(const CommandPool& commandPool);
		static void EndSingleTimeCommands(const CommandPool& commandPool, VkCommandBuffer commandBuffer);

	private:

		void CopyBuffer(const CommandPool& commandPool, Buffer& srcBuffer, VkDeviceSize size) const;
	};
}
