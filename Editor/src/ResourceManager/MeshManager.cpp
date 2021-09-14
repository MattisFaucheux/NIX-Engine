#include "ResourceManager/MeshManager.h"

#include "Log/Logger.h"
#include "ResourceManager/MainResourceManager.h"

namespace NIX::Editor
{
	void MeshManager::Clear()
	{
		for (auto resource : m_resourceStore)
		{
			delete resource.second;
		}
	}

	std::pair<bool, GeometricTools::Mesh*> MeshManager::LoadFile(const char* filename)
	{
		GeometricTools::Mesh* mesh = new GeometricTools::Mesh;
		bool result = mesh->LoadMesh(m_meshImporter, filename);

		if (!result)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "Failed to load mesh %s", filename);

			result = mesh->LoadMesh(m_meshImporter, FAILED_MESH_PATH);

			if (!result)
			{
				LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "Failed to load failed mesh resource");
				return { false, nullptr };
			}
		}

		mesh->m_path = filename;
		
		return std::pair<bool, GeometricTools::Mesh*>(result,mesh);
	}
}
