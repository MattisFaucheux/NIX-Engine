#pragma once
#include "DLLExport.h"

#include <PxPhysicsAPI.h>
#include <utility> //std::pair

#include"CollisionShape.h"

namespace NIX::Physic
{
	using namespace physx;

	class CollisionCapsule : public CollisionShape
	{
	public:

		CollisionCapsule() = delete;
		PHYSIC_LIBRARY_API CollisionCapsule(PxPhysics* physic, const PxMaterial& physicMat, float radius, float height);
		CollisionCapsule(const CollisionCapsule& other) = delete;
		CollisionCapsule(CollisionCapsule&& other) noexcept = delete;
		CollisionCapsule& operator=(const CollisionCapsule& other) = delete;
		CollisionCapsule& operator=(CollisionCapsule&& other) noexcept = delete;
		~CollisionCapsule() = default;

		PHYSIC_LIBRARY_API void SetRadius(float radius);
		PHYSIC_LIBRARY_API void SetHeight(float height);
		PHYSIC_LIBRARY_API void SetGeometry(float radius, float height);

		PHYSIC_LIBRARY_API [[nodiscard]] float GetRadius() const;
		PHYSIC_LIBRARY_API [[nodiscard]] float GetHeight() const;
		PHYSIC_LIBRARY_API [[nodiscard]] std::pair<float, float> GetRadiusAndHeight() const;
		
	private:

		void UpdateGeometry();

		float m_radius;
		float m_height;
	};
}
