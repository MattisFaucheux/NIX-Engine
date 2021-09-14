#pragma once
#include <string>
#include <vulkan/vulkan_core.h>
#include "API/DLLExportRendering.h"

namespace NIX::Rendering
{
	class TextureView
	{
	private:
		VkImageView m_imageView = nullptr;
		void* m_uiTexture = nullptr;

		std::string m_path = "";
		
		VkImage m_imageRef = nullptr;
		VkFormat m_format;
		VkImageAspectFlags m_aspectFlags;
	public:
		
		TextureView() = default;
		RENDERING_LIBRARY_API TextureView(const TextureView& other);
		TextureView(TextureView&& other) noexcept = default;
		TextureView& operator=(const TextureView& other) = delete;
		TextureView& operator=(TextureView&& other) noexcept = delete;
		RENDERING_LIBRARY_API ~TextureView();

		RENDERING_LIBRARY_API TextureView(const char* filePath);
		
		bool Initialize(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
		void Cleanup();

		RENDERING_LIBRARY_API std::string_view GetPath() const;
		
		[[nodiscard]] VkImageView GetVkImageView() const;
		RENDERING_LIBRARY_API [[nodiscard]] void*& GetUIImageID();

	private:
		bool CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	};

}
