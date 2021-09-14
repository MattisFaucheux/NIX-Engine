#include "pch.h"

#include "ResourceManager/TResourceManager.h"
#include "ResourceManager/MainResourceManager.h"
#include "ResourceManager/TextureViewManager.h"
#include "ResourceManager/MaterialManager.h"
#include "ResourceManager/MeshManager.h"

#include <cassert>
#include <string>
#include <filesystem>

namespace NIX::Editor
{
	MainResourceManager::MainResourceManager()
	{
		if (m_resourceManager != nullptr)
		{
			assert(false);
			return;
		}

		m_resourceManager = this;

		m_textureViewManager = new TextureViewManager();
		m_materialManager = new MaterialManager();
		m_meshManager = new MeshManager();
	}

	MainResourceManager::~MainResourceManager()
	{
		delete m_meshManager;
		delete m_materialManager;
		delete m_textureViewManager;
	}

	MainResourceManager* MainResourceManager::GetResourceManager()
	{
		return m_resourceManager;
	}

	TextureViewManager* MainResourceManager::GetTextureViewManager()
	{
		return m_resourceManager->m_textureViewManager;
	}

	MaterialManager* MainResourceManager::GetMaterialManager()
	{
		return m_resourceManager->m_materialManager;
	}

	MeshManager* MainResourceManager::GetMeshManager()
	{
		return m_resourceManager->m_meshManager;
	}

	void*& MainResourceManager::GetUiTextureId(const std::string& imagePath)
	{
		std::filesystem::path filePath(imagePath);

		if (filePath.extension() == ".mat" && !GetMaterialManager()->Load(imagePath.c_str()))
		{
			return m_resourceManager->m_textureViewManager->Find(FAILED_MAT_PATH)->GetUIImageID();
		}

		return m_resourceManager->m_textureViewManager->Find(imagePath.c_str())->GetUIImageID();
	}
}
