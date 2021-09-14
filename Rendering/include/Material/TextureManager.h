#pragma once
#include <map>
#include <string>
#include <vulkan/vulkan_core.h>

namespace NIX::Rendering
{
	class CommandPool;
	class Texture;

	class TextureManager
	{
	private:
		inline static CommandPool* m_commandPool = nullptr;
		inline static std::map<std::string, Texture*> m_textures;
	public:
		TextureManager() = default;
		TextureManager(const TextureManager& other) = delete;
		TextureManager(TextureManager&& other) noexcept = delete;
		TextureManager& operator=(const TextureManager& other) = delete;
		TextureManager& operator=(TextureManager&& other) noexcept = delete;
		~TextureManager();
		
		TextureManager(CommandPool* commandPool);

		static VkImage LoadTexture(const std::string& texturePath);
	};
}
