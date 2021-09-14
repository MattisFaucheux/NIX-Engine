#pragma once

#include "Memory/Buffer.h"
#include "Vector/Vector3.h"

#include <vector>

#include "MeshImport/Mesh.h"

namespace NIX {
	namespace GeometricTools {
		class Mesh;
		struct Vertex;
	}
}

namespace NIX::Rendering
{
	struct RenderingMesh
	{
		Buffer vertexBuffer;
		Buffer indexBuffer;

		size_t vertexCount = 0;
		size_t indexCount = 0;

		std::vector<GeometricTools::Vertex> vertices;
		std::vector<LibMath::Vector3> points;

		RenderingMesh() = delete;
		RenderingMesh(const RenderingMesh& other) = delete;
		RenderingMesh(RenderingMesh&& other) noexcept = delete;
		RenderingMesh& operator=(const RenderingMesh& other) = delete;
		RenderingMesh& operator=(RenderingMesh&& other) noexcept = delete;
		~RenderingMesh() = default;

		RenderingMesh(const CommandPool& commandPool, const GeometricTools::Mesh& mesh);
		RenderingMesh(const CommandPool& commandPool, const std::vector<LibMath::Vector3>& points);

		void Update(const CommandPool& commandPool, const GeometricTools::Mesh& mesh);
		void ChangeVertex(const CommandPool& commandPool, const std::vector<LibMath::Vector3>& points);
	};
}
