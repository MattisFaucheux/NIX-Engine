#pragma once
#include <string>

namespace NIX::Editor
{
	class LuaSceneManager
	{
	public:

		static void LuaInit();

		static void LoadSceneWithName(std::string sceneName);
		static void LoadSceneWithId(int sceneId);

		static std::string GetCurrentSceneName();
		static int GetCurrentSceneId();

	};

}
