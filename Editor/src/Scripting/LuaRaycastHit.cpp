#include "pch.h"

#include "Scripting/LuaRaycastHit.h"
#include "Scripting/ScriptManager.h"

#include "RaycastHit.h"
#include "GameObject.h"
#include "SceneManager.h"

namespace NIX::Editor
{
	void LuaRaycastHit::LuaInit()
	{
		ScriptManager::GetSingleton()->AddClass<Physic::RaycastHit, void>("RaycastHit");
		ScriptManager::GetSingleton()->AddClassFunction<Physic::RaycastHit>("RaycastHit", "GetDistance", &Physic::RaycastHit::GetDistance);
		ScriptManager::GetSingleton()->AddClassFunction<Physic::RaycastHit>("RaycastHit", "GetPosition", &Physic::RaycastHit::GetPosition);
		ScriptManager::GetSingleton()->AddClassFunction<Physic::RaycastHit>("RaycastHit", "GetNormal", &Physic::RaycastHit::GetNormal);
		ScriptManager::GetSingleton()->AddClassFunction<Physic::RaycastHit>("RaycastHit", "hadInitialOverlapp", &Physic::RaycastHit::hadInitialOverlapp);

		ScriptManager::GetSingleton()->AddFunctionInNamespace("RaycastHit", "GetGameObject", LuaRaycastHit::GetGameObject);
	}

	GameObject* LuaRaycastHit::GetGameObject(const Physic::RaycastHit& raycastHit)
	{
		return SceneManager::GetActiveScene()->sceneRoot->GetCollisionShapeGameObject(raycastHit.GetShape());
	}
}