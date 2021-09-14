#include "pch.h"

#include "MeshImport/Mesh.h"

#include <cassert>
#include <filesystem>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>

#include "MeshImport/MeshImporter.h"
#include "Log/Logger.h"


namespace NIX::GeometricTools
{
	bool Mesh::LoadMesh(MeshImporter& meshImporter, const std::string& Filename)
	{		
		if (!std::filesystem::exists(Filename))
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "Loading mesh failed, file : %s does't exist.", Filename.c_str());
			return false;
		}
		
		m_path = Filename;
		
		Clear();

		
		const aiScene* pScene = meshImporter.importer->ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_PreTransformVertices);

		if (pScene) 
		{
			InitFromScene(pScene, Filename);
			return true;
		}
		else 
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "Error parsing mesh : '%s': '%s'\n", Filename.c_str(), meshImporter.importer->GetErrorString());
			return false;
		}
	}

	void Mesh::InitFromScene(const aiScene* pScene, const std::string& Filename)
	{
		m_meshFrags.resize(pScene->mNumMeshes);
		m_texturesPath.resize(pScene->mNumMaterials);

		for (unsigned int i = 0; i < m_meshFrags.size(); i++) 
		{
			const aiMesh* paiMesh = pScene->mMeshes[i];
			InitMesh(i, paiMesh);
		}

		InitTextures(pScene, Filename);
	}

	void Mesh::InitMesh(unsigned int Index, const aiMesh* paiMesh)
	{
		std::vector<Vertex> vertices;
		std::vector<int> indices;

		const aiVector3D zero3D(0.0f, 0.0f, 0.0f);

		for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) 
		{
			const aiVector3D* pPos = &(paiMesh->mVertices[i]);
			const aiVector3D* pNormal = paiMesh->HasNormals() ? &(paiMesh->mNormals[i]) : &zero3D;
			const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &zero3D;

			Vertex v;
			v.position = { pPos->x, pPos->y, pPos->z };
			v.normal = { pNormal->x, pNormal->y, pNormal->z };
			v.texCord = LibMath::Vector2{ pTexCoord->x, pTexCoord->y };
				
			vertices.push_back(v);
		}

		for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) 
		{
			const aiFace& Face = paiMesh->mFaces[i];
			assert(Face.mNumIndices == 3);
			indices.push_back(Face.mIndices[0]);
			indices.push_back(Face.mIndices[1]);
			indices.push_back(Face.mIndices[2]);
		}

		m_meshFrags[Index] = {vertices, indices, paiMesh->mMaterialIndex};
	}

	void Mesh::InitMaterials(const aiScene* pScene, const std::string& /*Filename*/)
	{
		for (unsigned int i = 0; i < pScene->mNumMaterials; i++)
		{
			const aiMaterial* pMaterial = pScene->mMaterials[i];
			
			//TO DO: Define a material struct
			if (pMaterial->GetTextureCount(aiTextureType_NORMALS) > 0) 
			{
				aiString Path;
				if (pMaterial->GetTexture(aiTextureType_NORMALS, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
				{
					m_texturesPath[i] = Path.data;
				}
			}
		}
	}

	void Mesh::InitTextures(const aiScene* pScene, const std::string& /*Filename*/)
	{
		for (unsigned int i = 0; i < pScene->mNumTextures; i++)
		{
			const aiTexture* pTexture = pScene->mTextures[i];

			m_texturesPath[i] = pTexture->mFilename.data;
		}
	}

	void Mesh::Clear()
	{
		m_meshFrags.clear();
		m_texturesPath.clear();
	}

	Vertex::Vertex(const LibMath::Vector3& pPosition, const LibMath::Vector3& pNormal, const LibMath::Vector2& pTexCord)
		: position(pPosition), normal(pNormal), texCord(pTexCord)
	{
	}

	MeshFrag::MeshFrag(const std::vector<Vertex>& pVertices, const std::vector<int>& pIndices, unsigned int pMaterialIndex)
		: vertices(pVertices), indices(pIndices), materialIndex(pMaterialIndex)
	{}

}
