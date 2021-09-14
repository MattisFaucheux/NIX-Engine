#pragma once

#include <string>

namespace NIX::Editor
{
	class GameObject;

	class LuaScene
	{
	public:

		static void LuaInit();

		//static GameObject* GetGameObjectWithName(std::string name, int index);
		//static int GetGameObjectWithNameCount(std::string name);

		static GameObject* GetGameObjectWithTag(std::string tag, int index);
		static int GetGameObjectWithTagCount(std::string tag);

		static GameObject* CreateGameObjectEmpty(std::string name = "GameObject");
		static GameObject* CreateGameObjectCube(std::string name = "GameObject");
		static GameObject* CreateGameObjectSphere(std::string name = "GameObject");
		static GameObject* DuplicateGameObject(GameObject* otherGameObject, std::string name = "GameObject");


	};

}