#pragma once

#include "Vector/Vector3.h"

namespace NIX::Physic
{
	struct RaycastHitBuffer;
}

namespace NIX::Editor
{
	class LuaRaycast
	{
	public:

		static void LuaInit();

		static Physic::RaycastHitBuffer LaunchRaycast(const LibMath::Vector3& origin, const LibMath::Vector3& dir, float maxDistance);
	};
}