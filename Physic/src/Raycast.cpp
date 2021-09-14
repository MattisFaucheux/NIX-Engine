#include "pch.h"

#include "Raycast.h"
#include "PhysicSimulation.h"

#include <PxPhysicsAPI.h>

namespace NIX::Physic
{
	bool Raycast::LaunchRaycast(const LibMath::Vector3& origin, const LibMath::Vector3& dir, float maxDistance, RaycastHitBuffer& hitBuffer)
	{
		bool result = PhysicSimulation::GetSingleton()->GetCurrentScene()->GetScene()->raycast({ origin.x, origin.y, origin.z }, { dir.x, dir.y, dir.z }, maxDistance, hitBuffer.GetOriginalBuffer());
		hitBuffer.UpdateRaycastHitInfo();

		return result;
	}
}
