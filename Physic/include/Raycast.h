#pragma once
#include "DLLExport.h"

#include "RaycastHitBuffer.h"
#include "Vector/Vector3.h"

namespace NIX::Physic
{
	struct Raycast
	{
		PHYSIC_LIBRARY_API static bool LaunchRaycast(const LibMath::Vector3& origin, const LibMath::Vector3& dir, float maxDistance, RaycastHitBuffer& hitBuffer);
	};
}

