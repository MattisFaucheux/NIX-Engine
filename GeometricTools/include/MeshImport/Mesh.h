#pragma once
#include "API/DLLExportGeometricTools.h"

#include <array>
#include <string>
#include <vector>

#include "Vector/Vector3.h"
#include "Vector/TVector2.h"

namespace Assimp {
	class Importer;
}

struct aiScene;
struct aiMesh;

namespace NIX::GeometricTools
{
	struct MeshImporter;

	struct Point
	{
		LibMath::Vector3 position;
		LibMath::Vector3 color;
	};
	
	struct Vertex
	{		
		LibMath::Vector3 position;
		LibMath::Vector3 normal;
		LibMath::Vector2 texCord;

		Vertex() = default;
		GEOMETRIC_TOOLS_LIBRARY_API Vertex(const LibMath::Vector3& pPosition, const LibMath::Vector3& pNormal = { 0.0f, 0.0f, 0.0f }, const LibMath::Vector2& pTexCord = LibMath::Vector2{  0.0f, 0.0f });

	};

	struct MeshFrag {

		GEOMETRIC_TOOLS_LIBRARY_API MeshFrag(const std::vector<Vertex>& pVertices, const std::vector<int>& pIndices, unsigned int pMaterialIndex);

		MeshFrag() = default;
		~MeshFrag() = default;

		std::vector<Vertex> vertices;
		std::vector<int> indices;
		unsigned int materialIndex = 0;
	};
	
	class Mesh
	{
	public:

		Mesh() = default;
		~Mesh() = default;

		GEOMETRIC_TOOLS_LIBRARY_API bool LoadMesh(MeshImporter& meshImporter, const std::string& Filename);
		std::vector<MeshFrag> m_meshFrags;
		std::vector<std::string> m_texturesPath;

		std::string m_path = "";
	private:

        void InitFromScene(const aiScene* pScene, const std::string& Filename);
        void InitMesh(unsigned int Index, const aiMesh* paiMesh);
		
        void InitMaterials(const aiScene* pScene, const std::string& Filename);
		void InitTextures(const aiScene* pScene, const std::string& Filename);
        void Clear();

		
	};

}
