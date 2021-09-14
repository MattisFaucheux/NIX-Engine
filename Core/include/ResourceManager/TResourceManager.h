#pragma once

#include <string>
#include <map>
#include <utility>

#include <string>

namespace GeometricTools
{
	class Mesh;
}

namespace NIX::Core
{
	template <class ResourceType>
	class TResourceManager
	{
	public:

		TResourceManager() = default;
		
		TResourceManager(const TResourceManager& other) = delete;
		TResourceManager(TResourceManager&& other) noexcept = delete;
		TResourceManager& operator=(const TResourceManager& other) = delete;
		TResourceManager& operator=(TResourceManager&& other) noexcept = delete;
		virtual ~TResourceManager() = default;
		

		ResourceType Find(const char* filename)
		{
			std::string name = filename;

			if (name == "Cube")
				name = "Resources\\Meshes\\Cube.obj";
			else if (name == "Sphere")
				name = "Resources\\Meshes\\Sphere.obj";
			else if (name == "Default Texture")
				name = "Resources\\Textures\\defaultTexture.jpg";
			else if (name == "Default Material")
				name = "Resources\\Materials\\defaultMaterial.mat";

			if(Load(name.c_str()))
			{
				return m_resourceStore[name.c_str()];
			}

			return nullptr;
		}

		[[nodiscard]] bool Load(const char* filename)
		{
			const std::map<std::string, ResourceType>::iterator it = m_resourceStore.find(filename);

			if (it == m_resourceStore.end())
			{
				return AddResource(filename);
			}

			return true;
		}

		virtual bool AddResource(ResourceType resource, const char* filename)
		{
			auto result = m_resourceStore.try_emplace(filename, resource);

			return result.second;
		}

		virtual void Clear() = 0;
		
	
	private:

		[[nodiscard]] virtual bool AddResource(const char* filename)
		{
			std::pair<bool, ResourceType> result = LoadFile(filename);
			if(result.first)
			{
				m_resourceStore[filename] = result.second;
				return true;
			}

			return false;
		}

		virtual std::pair<bool, ResourceType> LoadFile(const char* filename) = 0;

	protected:
		std::map<std::string, ResourceType> m_resourceStore;
	};
}




