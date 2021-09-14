#pragma once
#include "DLLExport.h"

#include <PxPhysicsAPI.h>

#include "CollisionShape.h"
#include "RigidBody.h"

#include "Vector/Vector3.h"

namespace NIX::Physic
{
	class PhysicSimulation;
	using namespace physx;

	struct RaycastHit
	{
		PHYSIC_LIBRARY_API [[nodiscard]] float GetDistance() const;
		PHYSIC_LIBRARY_API [[nodiscard]] LibMath::Vector3 GetPosition() const;
		PHYSIC_LIBRARY_API [[nodiscard]] LibMath::Vector3 GetNormal() const;
		PHYSIC_LIBRARY_API [[nodiscard]] CollisionShape* GetShape() const;
		//PHYSIC_LIBRARY_API [[nodiscard]] RigidBody* GetRigidBody() const;
		PHYSIC_LIBRARY_API [[nodiscard]] bool hadInitialOverlapp() const;

		void SetRaycastHitInfo(PxRaycastHit* raycastHit);

	private:
	
		PxRaycastHit* originalHit;
		CollisionShape* shape = nullptr;
		//RigidBody* rigidBody = nullptr;
	};
}