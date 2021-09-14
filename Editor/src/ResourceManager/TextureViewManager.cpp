#include "pch.h"

#include "ResourceManager/TextureViewManager.h"

#include <string>

namespace NIX::Editor
{
	void TextureViewManager::Clear()
	{
		for (auto& resource : m_resourceStore)
		{
			delete resource.second;
		}
	}

	void*& TextureViewManager::GetUITextureID(const std::string& imagePath)
	{
		return Find(imagePath.c_str())->GetUIImageID();
	}

	void TextureViewManager::UpdateTexturePtr(std::string_view path, Rendering::TextureView* newPtr)
	{		
		std::map<std::string, Rendering::TextureView*>::iterator it = m_resourceStore.find(path.data());
		
		if (it != m_resourceStore.end())
		{
			delete it->second;
			it->second = newPtr;
		}
	}

	std::pair<bool, Rendering::TextureView*> TextureViewManager::LoadFile(const char* filename)
	{
		Rendering::TextureView* textureView = new Rendering::TextureView(filename);

		if (textureView == nullptr)
		{
			return { false, nullptr };
		}

		return std::pair<bool, Rendering::TextureView*>(true, textureView);
	}
}


