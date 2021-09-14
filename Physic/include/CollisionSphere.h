#pragma once
#include "DLLExport.h"

#include <PxPhysicsAPI.h>

#include"CollisionShape.h"

namespace NIX::Physic
{
	using namespace physx;

	class CollisionSphere: public CollisionShape
	{
	public:

		CollisionSphere() = delete;
		PHYSIC_LIBRARY_API CollisionSphere(PxPhysics* physic, const PxMaterial& physicMat, float radius);
		CollisionSphere(const CollisionSphere& other) = delete;
		CollisionSphere(CollisionSphere&& other) noexcept = delete;
		CollisionSphere& operator=(const CollisionSphere& other) = delete;
		CollisionSphere& operator=(CollisionSphere&& other) noexcept = delete;
		~CollisionSphere() = default;

		PHYSIC_LIBRARY_API void SetRadius(float radius);
		PHYSIC_LIBRARY_API [[nodiscard]] float GetRadius() const;

	private:

		void UpdateGeometry();

		float m_radius;
	};
}
