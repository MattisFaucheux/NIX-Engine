#pragma once

#include "API/DLLExportGeometricTools.h"

namespace Assimp {
	class Importer;
}

namespace NIX::GeometricTools
{
	struct MeshImporter
	{
		Assimp::Importer* importer;

		GEOMETRIC_TOOLS_LIBRARY_API MeshImporter();
		GEOMETRIC_TOOLS_LIBRARY_API ~MeshImporter();
	};
}
