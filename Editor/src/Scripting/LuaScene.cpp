#include "pch.h"

#include "Scripting/LuaScene.h"
#include "Scripting/ScriptManager.h"

#include "GameObject.h"
#include "SceneManager.h"
#include "UI/UIEditor.h"

#include "Components/CollisionBox.h"
#include "Components/CollisionSphere.h"

namespace NIX::Editor
{
	void LuaScene::LuaInit()
	{
		//ScriptManager::GetSingleton()->AddFunctionInNamespace("Scene", "GetGameObjectWithName", LuaScene::GetGameObjectWithName);
		//ScriptManager::GetSingleton()->AddFunctionInNamespace("Scene", "GetGameObjectWithNameCount", LuaScene::GetGameObjectWithNameCount);

		ScriptManager::GetSingleton()->AddFunctionInNamespace("Scene", "GetGameObjectWithTag", LuaScene::GetGameObjectWithTag);
		ScriptManager::GetSingleton()->AddFunctionInNamespace("Scene", "GetGameObjectWithTagCount", LuaScene::GetGameObjectWithTagCount);
		ScriptManager::GetSingleton()->AddFunctionInNamespace("Scene", "CreateGameObjectEmpty", LuaScene::CreateGameObjectEmpty);
		ScriptManager::GetSingleton()->AddFunctionInNamespace("Scene", "CreateGameObjectCube", LuaScene::CreateGameObjectCube);
		ScriptManager::GetSingleton()->AddFunctionInNamespace("Scene", "CreateGameObjectSphere", LuaScene::CreateGameObjectSphere);
		ScriptManager::GetSingleton()->AddFunctionInNamespace("Scene", "DuplicateGameObject", LuaScene::DuplicateGameObject);
	}

	//GameObject* LuaScene::GetGameObjectWithName(std::string name, int index)
	//{
	//	return nullptr;
	//}

	//int LuaScene::GetGameObjectWithNameCount(std::string name)
	//{
	//	return 0;
	//}

	GameObject* LuaScene::GetGameObjectWithTag(std::string tag, int index)
	{
		return UIEditor::GetInspectorWindow().GetObjectsWithTag(tag)[index];
	}

	int LuaScene::GetGameObjectWithTagCount(std::string tag)
	{
		return static_cast<int>(UIEditor::GetInspectorWindow().GetObjectsWithTag(tag).size());
	}

	GameObject* LuaScene::CreateGameObjectEmpty(std::string name)
	{
		GameObject* empty = new GameObject(name);
		empty->SetParent(SceneManager::GetActiveScene()->sceneRoot);
		return empty;
	}

	GameObject* LuaScene::CreateGameObjectCube(std::string name)
	{
		GameObject* cube = new GameObject(name);
		cube->SetParent(SceneManager::GetActiveScene()->sceneRoot);
		cube->AddComponent(new Model(cube, "Cube"));
		cube->AddComponent(new CollisionBox(cube, cube->GetTransform()->GetWorldScale()));
		return cube;
	}

	GameObject* LuaScene::CreateGameObjectSphere(std::string name)
	{
		GameObject* sphere = new GameObject(name);
		sphere->SetParent(SceneManager::GetActiveScene()->sceneRoot);
		sphere->AddComponent(new Model(sphere, "Sphere"));
		sphere->AddComponent(new CollisionSphere(sphere, 1.0f));
		return sphere;
	}

	GameObject* LuaScene::DuplicateGameObject(GameObject* otherGameObject, std::string name)
	{
		GameObject* duplicate = new GameObject(otherGameObject->GetParent(), *otherGameObject);
		duplicate->SetName(name);
		otherGameObject->GetParent()->GetChildren().emplace_back(duplicate);
		return duplicate;
	}
}