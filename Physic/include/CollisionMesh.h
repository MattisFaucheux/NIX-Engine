#pragma once
#include "DLLExport.h"

#include <PxPhysicsAPI.h>

#include"CollisionShape.h"
#include<vector>
#include <Vector/Vector3.h>

namespace NIX::Physic
{
	using namespace physx;

	class CollisionMesh : public CollisionShape
	{
	public:
		CollisionMesh() = delete;
		PHYSIC_LIBRARY_API CollisionMesh(PxPhysics* physic, const PxMaterial& physicMat, PxConvexMesh* convexMesh, const std::vector<LibMath::Vector3>& points, const LibMath::Vector3& scale);
		CollisionMesh(const CollisionMesh& other) = delete;
		CollisionMesh(CollisionMesh&& other) noexcept = delete;
		CollisionMesh& operator=(const CollisionMesh& other) = delete;
		CollisionMesh& operator=(CollisionMesh&& other) noexcept = delete;
		~CollisionMesh() = default;

		PHYSIC_LIBRARY_API void SetGeometry(PxConvexMesh* convexMesh);
		PHYSIC_LIBRARY_API void SetGeometry(const std::vector<LibMath::Vector3>& points, const LibMath::Vector3& scale);

		PHYSIC_LIBRARY_API void SetScale(const LibMath::Vector3& scale);
		PHYSIC_LIBRARY_API [[nodiscard]] const LibMath::Vector3& GetScale() const;

		PHYSIC_LIBRARY_API void SetPoints(const std::vector<LibMath::Vector3>& points);
		PHYSIC_LIBRARY_API [[nodiscard]]  const std::vector<LibMath::Vector3>& GetPoints() const;
		
	private:

		void UpdateGeometry();
		
		LibMath::Vector3 m_scale;
		PxConvexMesh* m_convexMesh;

		std::vector<LibMath::Vector3> m_points;
	};
}
