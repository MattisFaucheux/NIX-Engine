#pragma once


#include "ResourceManager/TResourceManager.h"
#include "MeshImport/Mesh.h"
#include "MeshImport/MeshImporter.h"

#define FAILED_MESH_PATH "Resources\\Meshes\\Cube.obj"

namespace NIX
{
	namespace Editor
	{
		class MeshManager : public Core::TResourceManager<GeometricTools::Mesh*>
		{
		public:
			GeometricTools::MeshImporter m_meshImporter;
			
			void Clear() override;
		private:
			std::pair<bool, GeometricTools::Mesh*> LoadFile(const char* filename) override;
		};
	}
}
