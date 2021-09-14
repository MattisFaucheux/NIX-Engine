#include "pch.h"

#include "RaycastHit.h"
#include "PhysicSimulation.h"

namespace NIX::Physic
{
	float RaycastHit::GetDistance() const
	{
		return originalHit->distance;
	}

	LibMath::Vector3 RaycastHit::GetPosition() const
	{
		PxVec3 pos = originalHit->position;
		return { pos.x, pos.y, pos.z };
	}

	LibMath::Vector3 RaycastHit::GetNormal() const
	{
		PxVec3 norm = originalHit->normal;
		return { norm.x, norm.y, norm.z };
	}

	CollisionShape* RaycastHit::GetShape() const
	{
		return shape;
	}

	bool RaycastHit::hadInitialOverlapp() const
	{
		return originalHit->hadInitialOverlap();
	}

	void RaycastHit::SetRaycastHitInfo(PxRaycastHit* raycastHit)
	{
		originalHit = raycastHit;

		shape = reinterpret_cast<CollisionShape*>(raycastHit->shape->userData);
	}
}
