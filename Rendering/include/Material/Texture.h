#pragma once

#include "Memory/DeviceMemory.h"

#include <vulkan/vulkan_core.h>
#include <string>

#define FAILED_TEXTURE_PATH "Resources\\Textures\\failedTexture.jpg"

namespace NIX::Rendering
{
	class Buffer;
	class SwapChain;
	class CommandPool;
	class PhysicalDevice;
	class LogicalDevice;

	class Texture
	{
	private:
		VkImage m_image;
		DeviceMemory m_imageMemory;
		
	public:
		Texture() = default;
		Texture(const Texture& other) = default;
		Texture(Texture&& other) noexcept = default;
		Texture& operator=(const Texture& other) = delete;
		Texture& operator=(Texture&& other) noexcept = delete;
		~Texture();

		Texture(const CommandPool& commandPool, const std::string& filePath);
		
		void Initialize(const SwapChain& swapChain);
		void Cleanup();
		
		[[nodiscard]] VkImage GetVkImage() const;

	private:
		void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
		
		void TransitionImageLayout(const CommandPool& commandPool, VkImageLayout oldLayout, VkImageLayout newLayout);
		
		void CopyBufferToImage(const CommandPool& commandPool, Buffer& buffer, uint32_t width, uint32_t height);
	};
}
