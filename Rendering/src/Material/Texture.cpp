#include "pch.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#include "Tools/Initializers.h"
#include "Material/Texture.h"
#include "Memory/Buffer.h"
#include "Context/Device/Device.h"
#include "SwapChain/SwapChain.h"
#include "CommandPool.h"

#include <stdexcept>
#include <vulkan/vulkan_core.h>

#include "Log/Logger.h"

namespace NIX::Rendering
{
	Texture::~Texture()
	{
        vkDestroyImage(Device::GetSingleton()->GetVkLogicalDevice(), m_image, nullptr);
	}

	Texture::Texture(const CommandPool& commandPool, const std::string& filePath)
	{
        int texWidth, texHeight, texChannels;
        stbi_uc* pixels = stbi_load(filePath.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        uint64_t imageSize = texWidth * texHeight * 4;

        if (!pixels)
        {
            LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "Failed to load texture image %s", filePath.c_str());
             pixels = stbi_load(FAILED_TEXTURE_PATH, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
           imageSize = texWidth * texHeight * 4;

            if (!pixels)
            {
                LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "Failed to load failed texture image");
                return;
            }
           
        }

        Buffer stagingBuffer;

        stagingBuffer.CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        stagingBuffer.Update(imageSize, pixels);

        stbi_image_free(pixels);

        CreateImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        TransitionImageLayout(commandPool, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        CopyBufferToImage(commandPool, stagingBuffer, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
        TransitionImageLayout(commandPool, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	}

    void Texture::Initialize(const SwapChain& swapChain)
    {
        CreateImage(swapChain.GetExtent().width, swapChain.GetExtent().height, Device::GetSingleton()->FindDepthFormat(), VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    }
	
    void Texture::Cleanup()
    {
        m_imageMemory.Cleanup();
        vkDestroyImage(Device::GetSingleton()->GetVkLogicalDevice(), m_image, nullptr);
    }
	

	VkImage Texture::GetVkImage() const
	{
        return m_image;
	}

	void Texture::CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties)
	{
        VkImageCreateInfo imageInfo = Initializers::ImageCreateInfo();;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = tiling;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = usage;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateImage(Device::GetSingleton()->GetVkLogicalDevice(), &imageInfo, nullptr, &m_image) != VK_SUCCESS) 
        {
            ASSERT(SOURCE_INFO, "Failed to create image!");
        }

        m_imageMemory.Initialize(m_image, properties);

        
	}
	void Texture::TransitionImageLayout(const CommandPool& commandPool, VkImageLayout oldLayout, VkImageLayout newLayout)
	{
        VkCommandBuffer commandBuffer = Buffer::BeginSingleTimeCommands(commandPool);

        VkImageMemoryBarrier barrier = Initializers::ImageMemoryBarrier();
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = m_image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;

        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) 
        {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) 
        {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        else 
        {
            throw std::invalid_argument("unsupported layout transition!");
        }

        vkCmdPipelineBarrier(
            commandBuffer,
            sourceStage, destinationStage,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier
        );

        Buffer::EndSingleTimeCommands(commandPool, commandBuffer);
	}

	void Texture::CopyBufferToImage(const CommandPool& commandPool, Buffer& buffer, uint32_t width, uint32_t height)
	{
        VkCommandBuffer commandBuffer = Buffer::BeginSingleTimeCommands(commandPool);

        VkBufferImageCopy region;
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;

        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;

        region.imageOffset = { 0, 0, 0 };
        region.imageExtent = {
            width,
            height,
            1
        };

        buffer.CopyBufferToImage(commandBuffer, m_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, region);
		
        Buffer::EndSingleTimeCommands(commandPool, commandBuffer);
	}
}
