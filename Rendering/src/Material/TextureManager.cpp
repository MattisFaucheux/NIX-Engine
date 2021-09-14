#include "pch.h"

#include "Material/TextureManager.h"
#include "Material/Texture.h"
#include "CommandPool.h"
#include "Log/Logger.h"

namespace NIX::Rendering
{
	TextureManager::TextureManager(CommandPool* commandPool)
	{
		m_commandPool = commandPool;
	}

	TextureManager::~TextureManager()
	{
		for (auto& texture : m_textures)
		{
			delete texture.second;
		}
	}

	VkImage TextureManager::LoadTexture(const std::string& texturePath)
	{
		if (texturePath.empty())
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "Failed to load texture, path was empty");
			return nullptr;
		}

		auto it = m_textures.find(texturePath);
		
		if (it == m_textures.end())
		{
			const auto result = m_textures.try_emplace(texturePath, new Texture(*m_commandPool, texturePath));

			if (!result.second)
			{
				LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "Failed to load texture %s", texturePath.c_str());
				return nullptr;
			}

			it = result.first;
		}
		
		return it->second->GetVkImage();
	}
}
