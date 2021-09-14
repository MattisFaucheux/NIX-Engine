#pragma once
#include "DLLExport.h"

#include <PxPhysicsAPI.h>

#include"CollisionShape.h"
#include "Vector/Vector3.h"

namespace NIX::Physic
{
	using namespace physx;

	class CollisionBox : public CollisionShape
	{
	public:

		CollisionBox() = delete;
		PHYSIC_LIBRARY_API CollisionBox(PxPhysics* physic, const PxMaterial& physicMat, const LibMath::Vector3& scale);
		CollisionBox(const CollisionBox& other) = delete;
		CollisionBox(CollisionBox&& other) noexcept = delete;
		CollisionBox& operator=(const CollisionBox& other) = delete;
		CollisionBox& operator=(CollisionBox&& other) noexcept = delete;
		~CollisionBox() = default;
		
		PHYSIC_LIBRARY_API void SetScaleX(float scaleX);
		PHYSIC_LIBRARY_API void SetScaleY(float scaleY);
		PHYSIC_LIBRARY_API void SetScaleZ(float scaleZ);
		PHYSIC_LIBRARY_API void SetScale(const LibMath::Vector3& scale);

		PHYSIC_LIBRARY_API [[nodiscard]] float GetScaleX() const;
		PHYSIC_LIBRARY_API [[nodiscard]] float GetScaleY() const;
		PHYSIC_LIBRARY_API [[nodiscard]] float GetScaleZ() const;
		PHYSIC_LIBRARY_API [[nodiscard]] LibMath::Vector3 GetScale() const;

		PHYSIC_LIBRARY_API void SetObjectScale(const LibMath::Vector3& scale);
		PHYSIC_LIBRARY_API [[nodiscard]] LibMath::Vector3 GetObjectScale() const;

	private:

		void UpdateGeometry();

		LibMath::Vector3 m_scale;
		LibMath::Vector3 m_objectScale = { 1, 1, 1 };
	};
}

	