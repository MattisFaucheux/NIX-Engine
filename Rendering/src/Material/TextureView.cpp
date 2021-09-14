#include "pch.h"

#include "Material/TextureView.h"

#include "Context/Context.h"
#include "Context/Device/Device.h"
#include "Tools/Initializers.h"
#include "Material/TextureManager.h"

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_vulkan.h"

namespace NIX::Rendering
{

	TextureView::TextureView(const char* filePath)
		: m_path(filePath)
	{
		Initialize(Context::GetTextureManager()->LoadTexture(filePath), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
	}

	TextureView::TextureView(const TextureView& other)
	{
		Initialize(other.m_imageRef, other.m_format, other.m_aspectFlags);
	}

	TextureView::~TextureView()
	{
		Cleanup();
	}

	bool TextureView::Initialize(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
	{
		m_imageRef = image;
		m_format = format;
		m_aspectFlags = aspectFlags;
		
		return CreateImageView(image, format, aspectFlags);
	}
	
	void TextureView::Cleanup()
	{
		vkDestroyImageView(Device::GetSingleton()->GetVkLogicalDevice(), m_imageView, nullptr);
	}


	std::string_view TextureView::GetPath() const
	{
		return m_path;
	}

	VkImageView TextureView::GetVkImageView() const
	{
        return m_imageView;
	}

	void*& TextureView::GetUIImageID()
	{
		if (m_uiTexture == nullptr)
		{
			m_uiTexture = ImGui_ImplVulkan_AddTexture(Context::GetTextureSampler().GetVkTextureSampler(), m_imageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		}
		
		return m_uiTexture;
	}

	bool TextureView::CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
	{
		VkImageViewCreateInfo viewInfo = Initializers::ImageViewCreateInfo();
        viewInfo.image = image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;
        viewInfo.subresourceRange.aspectMask = aspectFlags;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(Device::GetSingleton()->GetVkLogicalDevice(), &viewInfo, nullptr, &m_imageView) != VK_SUCCESS)
        {
			return false;
        }

		return true;
	}
}
