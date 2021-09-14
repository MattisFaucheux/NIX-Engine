#pragma once
#include "DLLExport.h"

#include <PxPhysicsAPI.h>

#include "Vector/Vector3.h"
#include "Quaternion/Quaternion.h"

#include "CollisionShape.h"


namespace NIX::Physic
{
	using namespace physx;

	class RigidStatic
	{
	public:

		RigidStatic() = delete;
		RigidStatic(PxPhysics* physic, PxScene* scene, CollisionShape* shape, const LibMath::Vector3& position);
		RigidStatic(const RigidStatic& other) = delete;
		RigidStatic(RigidStatic&& other) noexcept = delete;
		RigidStatic& operator=(const RigidStatic& other) = delete;
		RigidStatic& operator=(RigidStatic&& other) noexcept = delete;
		PHYSIC_LIBRARY_API ~RigidStatic();

		//Get RigidBody Position
		PHYSIC_LIBRARY_API [[nodiscard]] LibMath::Vector3 GetPosition() const;
		//Get Rigidbody Rotation
		PHYSIC_LIBRARY_API [[nodiscard]] LibMath::Quaternion GetRotation() const;

		PHYSIC_LIBRARY_API void SetPosition(const LibMath::Vector3& position);
		PHYSIC_LIBRARY_API void SetRotation(const LibMath::Quaternion& quaternion);
		PHYSIC_LIBRARY_API void SetTransform(const LibMath::Vector3& position, const LibMath::Quaternion& quaternion);

		//Get Collision Shape
		PHYSIC_LIBRARY_API [[nodiscard]] CollisionShape* GetCollisionShape() const;

		PHYSIC_LIBRARY_API void IsActive(bool isActive);
		PHYSIC_LIBRARY_API bool IsActive();

		[[nodiscard]] PxRigidStatic* GetActor();

	private:

		PxRigidStatic* m_actor = nullptr;
		CollisionShape* m_collisionShape = nullptr;
	};
}

