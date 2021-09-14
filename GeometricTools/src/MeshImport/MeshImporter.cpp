#include "pch.h"

#include "MeshImport/MeshImporter.h"
#include <assimp/Importer.hpp>

namespace NIX::GeometricTools
{
	MeshImporter::MeshImporter()
		: importer(new Assimp::Importer())
	{
		//importer.SetPropertyBool(AI_CONFIG_PP_PTV_KEEP_HIERARCHY, true);
		//importer.SetPropertyMatrix(AI_CONFIG_PP_PTV_ROOT_TRANSFORMATION, { {1.0f, 1.0f, 1.0f}, {-90.0f, 0.0f, 0.0f, 0.0f} , {0.0f, 0.0f, 0.0f} });
	}

	MeshImporter::~MeshImporter()
	{
		delete importer;
	}
}
