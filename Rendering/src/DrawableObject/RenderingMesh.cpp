#include "pch.h"

#include "DrawableObject/RenderingMesh.h"

#include "MeshImport/Mesh.h"
#include "Context/Device/Device.h"
#include "Memory/EBufferType.h"

#include "Vector/Vector3.h"

namespace NIX::Rendering
{
	
	RenderingMesh::RenderingMesh(const CommandPool& commandPool, const GeometricTools::Mesh& mesh)
	{
		std::vector<int> indices;

		int offset = 0;

		for (const auto& m_meshFrag : mesh.m_meshFrags)
		{
			for (const auto& vertex : m_meshFrag.vertices)
			{
				vertices.push_back(vertex);
				points.emplace_back(vertex.position[0], vertex.position[1], vertex.position[2]);
			}

			for (const auto indice : m_meshFrag.indices)
			{
				indices.push_back(indice + offset);
			}

			offset = static_cast<uint32_t>(vertices.size());
		}

		vertexBuffer.Initialize(commandPool, sizeof(GeometricTools::Vertex) * vertices.size(), vertices.data(), EBufferType::VERTEX);
		vertexCount = vertices.size();
		indexBuffer.Initialize(commandPool, sizeof(mesh.m_meshFrags[0].indices[0]) * indices.size(), indices.data(), EBufferType::INDEX);
		indexCount = indices.size();
	}

	RenderingMesh::RenderingMesh(const CommandPool& commandPool, const std::vector<LibMath::Vector3>& pPoints)
	{
				
		for (const auto& point : pPoints)
		{
			vertices.push_back(GeometricTools::Vertex({point.x, point.y, point.z }));
			points.emplace_back(point.x, point.y, point.z);
		}

		vertexBuffer.Initialize(commandPool, sizeof(GeometricTools::Vertex) * vertices.size(), vertices.data(), EBufferType::VERTEX);
		vertexCount = vertices.size();
		
		////////////////////////////////////////
		// TODO find a better solution
		// Avoid crash when clearing an empty indexBuffer

		int counterCrash = 0;
		
		indexBuffer.Initialize(commandPool, sizeof(int), &counterCrash, EBufferType::INDEX);
		indexCount = 0;

		////////////////////////////////////////
	}

	void RenderingMesh::Update(const CommandPool& commandPool, const GeometricTools::Mesh& mesh)
	{
		vertices.clear();
		std::vector<int> indices;

		int offset = 0;

		for (const auto& m_meshFrag : mesh.m_meshFrags)
		{
			for (const auto& vertex : m_meshFrag.vertices)
			{
				vertices.push_back(vertex);
			}

			for (const auto indice : m_meshFrag.indices)
			{
				indices.push_back(indice + offset);
			}

			offset = static_cast<uint32_t>(vertices.size());
		}

		vertexBuffer.Cleanup();
		vertexBuffer.Initialize(commandPool, sizeof(GeometricTools::Vertex) * vertices.size(), vertices.data(), EBufferType::VERTEX);
		vertexCount = vertices.size();
		indexBuffer.Cleanup();
		indexBuffer.Initialize(commandPool, sizeof(mesh.m_meshFrags[0].indices[0]) * indices.size(), indices.data(), EBufferType::INDEX);
		indexCount = indices.size();
	}

	void RenderingMesh::ChangeVertex(const CommandPool& commandPool, const std::vector<LibMath::Vector3>& pPoints)
	{
		vertices.clear();

		vkDeviceWaitIdle(Device::GetSingleton()->GetVkLogicalDevice());
		vertexBuffer.Cleanup();

		vertices.reserve(pPoints.size());
		for (const LibMath::Vector3& point : pPoints)
		{
			vertices.push_back(GeometricTools::Vertex({ point.x, point.y, point.z }));
		}
		
		vertexBuffer.Initialize(commandPool, sizeof(GeometricTools::Vertex) * vertices.size(), vertices.data(), EBufferType::VERTEX);
		vertexCount = vertices.size();

		
	}
}
