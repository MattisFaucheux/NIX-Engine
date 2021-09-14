#pragma once

#include "API/DLLExportRendering.h"

#include <vulkan/vulkan_core.h>


namespace NIX::Rendering
{
	class TextureSampler
	{
	private:
		VkSampler m_textureSampler;
	public:
		TextureSampler() = default;
		TextureSampler(const TextureSampler& other) = delete;
		TextureSampler(TextureSampler&& other) noexcept = delete;
		TextureSampler& operator=(const TextureSampler& other) = delete;
		TextureSampler& operator=(TextureSampler&& other) noexcept = delete;
		RENDERING_LIBRARY_API ~TextureSampler();
		
		void Initialize(bool anisotropyEnable);
		void Cleanup();

		[[nodiscard]] VkSampler GetVkTextureSampler() const;

	};
}
