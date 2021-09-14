#include "pch.h"

#include "Scripting\LuaSceneManager.h"
#include "Scripting/ScriptManager.h"

#include "SceneManager.h"
#include "Shipping/ShippingSettings.h"

namespace NIX::Editor
{
	void LuaSceneManager::LuaInit()
	{
		ScriptManager::GetSingleton()->AddFunctionInNamespace("SceneManager", "LoadSceneWithName", LuaSceneManager::LoadSceneWithName);
		ScriptManager::GetSingleton()->AddFunctionInNamespace("SceneManager", "LoadSceneWithId", LuaSceneManager::LoadSceneWithId);

		ScriptManager::GetSingleton()->AddFunctionInNamespace("SceneManager", "GetCurrentSceneName", LuaSceneManager::GetCurrentSceneName);
		ScriptManager::GetSingleton()->AddFunctionInNamespace("SceneManager", "GetCurrentSceneId", LuaSceneManager::GetCurrentSceneId);
	}

	void LuaSceneManager::LoadSceneWithName(std::string sceneName)
	{
		SceneManager::LoadScene(sceneName);
	}

	void LuaSceneManager::LoadSceneWithId(int sceneId)
	{
		SceneManager::LoadScene(sceneId);
	}

	std::string LuaSceneManager::GetCurrentSceneName()
	{
		return SceneManager::GetActiveSceneName();
	}

	int LuaSceneManager::GetCurrentSceneId()
	{
		return ShippingSettings::GetSceneId(SceneManager::GetActiveSceneName());
	}
}
