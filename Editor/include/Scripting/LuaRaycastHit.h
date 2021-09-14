#pragma once

#include "RaycastHit.h"

namespace NIX::Editor
{
	class GameObject;
}

namespace NIX::Editor
{
	class LuaRaycastHit
	{
	public:

		static void LuaInit();

		static GameObject* GetGameObject(const Physic::RaycastHit& raycastHit);
	};

}
