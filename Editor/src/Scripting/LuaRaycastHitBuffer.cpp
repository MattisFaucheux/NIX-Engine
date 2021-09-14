#include "pch.h"

#include "Scripting/LuaRaycastHitBuffer.h"
#include "Scripting/ScriptManager.h"

#include "RaycastHitBuffer.h"

namespace NIX::Editor
{
	void LuaRaycastHitBuffer::LuaInit()
	{
		ScriptManager::GetSingleton()->AddClass<Physic::RaycastHitBuffer, void>("RaycastHitBuffer");
		ScriptManager::GetSingleton()->AddClassFunction<Physic::RaycastHitBuffer>("RaycastHitBuffer", "HasBlock", &Physic::RaycastHitBuffer::HasBlock);
		ScriptManager::GetSingleton()->AddClassFunction<Physic::RaycastHitBuffer>("RaycastHitBuffer", "HasAnyHit", &Physic::RaycastHitBuffer::HasAnyHit);
		ScriptManager::GetSingleton()->AddClassFunction<Physic::RaycastHitBuffer>("RaycastHitBuffer", "GetNbAnyHits", &Physic::RaycastHitBuffer::GetNbAnyHits);
		ScriptManager::GetSingleton()->AddClassFunction<Physic::RaycastHitBuffer>("RaycastHitBuffer", "GetNbTouches", &Physic::RaycastHitBuffer::GetNbTouches);
		ScriptManager::GetSingleton()->AddClassFunction<Physic::RaycastHitBuffer>("RaycastHitBuffer", "GetBlockInfo", &Physic::RaycastHitBuffer::GetBlockInfo);
		ScriptManager::GetSingleton()->AddClassFunction<Physic::RaycastHitBuffer>("RaycastHitBuffer", "GetTouchInfo", &Physic::RaycastHitBuffer::GetTouchInfo);
	}
}