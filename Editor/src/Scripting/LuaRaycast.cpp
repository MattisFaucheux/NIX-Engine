#include "pch.h"

#include "Scripting/LuaRaycast.h"
#include "Scripting/ScriptManager.h"

#include "Raycast.h"

#include "Scripting/LuaRaycastHitBuffer.h"
#include "Scripting/LuaRaycastHit.h"
#include "RaycastHitBuffer.h"

namespace NIX::Editor
{
	void LuaRaycast::LuaInit()
	{
		LuaRaycastHit::LuaInit();
		LuaRaycastHitBuffer::LuaInit();

		ScriptManager::GetSingleton()->AddFunctionInNamespace("Raycast", "LaunchRaycast", LuaRaycast::LaunchRaycast);


	}

	Physic::RaycastHitBuffer LuaRaycast::LaunchRaycast(const LibMath::Vector3& origin, const LibMath::Vector3& dir, float maxDistance)
	{
		Physic::RaycastHitBuffer hitBuffer;
		Physic::Raycast::LaunchRaycast(origin, dir, maxDistance, hitBuffer);
		return hitBuffer;
	}
}