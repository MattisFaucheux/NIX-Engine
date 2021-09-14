#include "pch.h"

#include "ResourceManager/MaterialManager.h"

#include <filesystem>



#include "EditorApp.h"
#include "Log/Logger.h"
#include "Material/Material.h"
#include "ResourceManager/MainResourceManager.h"
#include "Serialization/AssetsParsing/JSonParser.h"
#include "Serialization/AssetsParsing/ParseMaterial.h"
#include "ResourceManager/TextureViewManager.h"

namespace NIX::Editor
{
	bool MaterialManager::AddResource(Rendering::Material* resource, const char* filename)
	{
		auto result = m_resourceStore.try_emplace(filename, resource);

		return result.second;
	}

	void MaterialManager::Clear()
	{
		for (auto resource : m_resourceStore)
		{
			delete resource.second;
		}
	}

	void MaterialManager::LoadAllMaterials()
	{
		const std::string materialDir = "Assets";

		for (const auto& entry : std::filesystem::recursive_directory_iterator(materialDir))
		{
			if (entry.path().extension() != ".mat")
			{
				continue;
			}
			std::string assetPath = entry.path().string();
			const std::pair<bool, Rendering::Material*> material = LoadFile(assetPath.c_str());
			
			if (material.first)
			{
				MainResourceManager::GetMaterialManager()->AddResource(material.second, assetPath.c_str());
			}
			else
			{
				LOG(SOURCE_INFO, Core::EVerbosity::LOG_WARNING, "Failed to load material : %s", assetPath.c_str());
			}
		}
	}


	std::pair<bool, Rendering::Material*> MaterialManager::LoadFile(const char* filename)
	{
		Core::ParseMaterial parseMaterial(filename);
		if (parseMaterial.albedoMap.empty())
		{
			if (LoadFailedMaterial(parseMaterial))
			{
				LOG(SOURCE_INFO, Core::EVerbosity::LOG_WARNING, "Material %s failed to load, default material load instead", filename);
				parseMaterial.albedoMap = filename;
			}			
		}

		Rendering::Material* material = nullptr;
		Rendering::TextureView* albedo = nullptr;
		Rendering::TextureView* normal = nullptr;

		
		if (!parseMaterial.albedoMap.empty())
		{
			albedo = MainResourceManager::GetTextureViewManager()->Find(parseMaterial.albedoMap.c_str());
		}
		if (!parseMaterial.normalMap.empty())
		{
			normal = MainResourceManager::GetTextureViewManager()->Find(parseMaterial.normalMap.c_str());
		}
		
		material = new Rendering::Material(albedo, normal);
		
		if (material == nullptr)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "Failed to create material %s !", filename);
			material = MainResourceManager::GetMaterialManager()->Find(FAILED_MAT_PATH);
			material->path = filename;
			return std::pair<bool, Rendering::Material*>(true, material);
		}

		Rendering::TextureView* preview = new Rendering::TextureView(parseMaterial.albedoMap.c_str());
		if (preview != nullptr)
		{
			if (!MainResourceManager::GetTextureViewManager()->AddResource(preview, filename))
			{
				LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "Failed to load material preview !");
			}
		}
		
		material->path = filename;

		return std::pair<bool, Rendering::Material*>(true, material);
	}

	bool MaterialManager::LoadFailedMaterial(Core::ParseMaterial& parseMaterial)
	{
		parseMaterial = Core::ParseMaterial(FAILED_MAT_PATH);
		if (parseMaterial.albedoMap.empty())
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "Failed to load default material, %s not found", FAILED_MAT_PATH);
			return false;
		}

		return true;
	}
}
