#include "SceneManager.h"

#include <filesystem>
#include <iostream>


#include "EditorApp.h"
#include "Log/Logger.h"
#include "UI/UIEditor.h"

#include "Camera/CameraManager.h"

namespace NIX::Editor
{
	Scene::~Scene()
	{
		delete sceneRoot;
		delete physicSimulation;
	}

	void Scene::IsActive(bool value)
	{
		if (sceneRoot == nullptr)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_WARNING, "Scene root = nullptr");
			return;
		}

		sceneRoot->IsActive(value);
	}

	void Scene::Update()
	{
		if (sceneRoot == nullptr)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_WARNING, "Scene root = nullptr");
			return;
		}

		sceneRoot->Update();
	}

	void Scene::SwitchToSimulation()
	{
		sceneRoot->SwitchToSimulation();
	}

	void Scene::ExitSimulation()
	{
		sceneRoot->ExitSimulation();
	}

	void Scene::SimulationUpdate(float deltaTime)
	{
		sceneRoot->SimulationUpdate(deltaTime);
	}

	void Scene::SimulationStart()
	{
		sceneRoot->SimulationStart();
	}

	void Scene::UpdateWorldMatrix()
	{
		sceneRoot->GetTransform()->UpdateWorldMatrix();
	}

	void Scene::Serialize(Core::Serializer& serializer, const std::string& scenePath)
	{
		serializer.ResetSerializer();
		
		Core::SaveElement sceneElem = serializer.AddRootElement(SCENE_ELEMENT);

		sceneRoot->Serialize(sceneElem);

		serializer.Serialize(scenePath, sceneElem);
	}

	SceneManager::SceneManager()
	{
		m_activeScene = m_editorScene;
	}

	void SceneManager::Init()
	{
		InitCamera();
		SetActiveScene(GetEditorScene());
		LoadScene(m_currentScenePath);
	}

	void SceneManager::InitCamera()
	{
		m_editorCamera = EditorApp::GetRenderingContext().GetCameraManager()->AddCamera();
		EditorApp::GetRenderingContext().SetCurrentCamera(m_editorCamera);
	}

	Rendering::Camera* SceneManager::GetEditorCamera()
	{
		return m_editorCamera;
	}

	void SceneManager::SetScenePath(std::string_view scene)
	{
		m_currentScenePath = scene;
	}

	std::string_view SceneManager::GetScenePath()
	{
		return m_currentScenePath;
	}

	void SceneManager::SetMainCamera(Rendering::Camera* camera)
	{
		m_mainCamera = camera;
	}

	Rendering::Camera* SceneManager::GetMainCamera()
	{
		return m_mainCamera;
	}

	void SceneManager::SetActiveScene(Scene* activeScene)
	{
		m_activeScene = activeScene;
	}

	Scene* SceneManager::GetActiveScene()
	{
		return m_activeScene;
	}

	Scene* SceneManager::GetEditorScene()
	{
		return m_editorScene;
	}

	Scene* SceneManager::GetSimulationScene()
	{
		return m_simulationScene;
	}

	void SceneManager::InitSimulation()
	{
#ifdef _SHIPPING
		if (EditorApp::GetShippingSettings().GetScenesInBuild().size() <= 0)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_FATAL, "No scene in build were saved");
		}
		m_simulationScene = new Scene();
		SetActiveScene(m_simulationScene);
		
		LoadScene(EditorApp::GetShippingSettings().GetScenesInBuild()[0]);
		LoadScene();
		
#endif
		
#ifndef _SHIPPING
		m_simulationScene = new Scene(m_editorScene->sceneRoot);
		m_editorScene->SwitchToSimulation();
		SetActiveScene(m_simulationScene);
#endif
		
	}

	void SceneManager::DestroySimulation()
	{
		delete m_simulationScene;
		
		Physic::PhysicSimulation::GetSingleton()->SetCurrentScene(m_editorScene->physicSimulation);
				
		m_editorScene->ExitSimulation();

		if (GetEditorScene() == nullptr)
		{
			return;
		}

		SetActiveScene(GetEditorScene());
		SetScenePath(m_editorScene->sceneName);

		GetEditorScene()->UpdateWorldMatrix();
	}

	void SceneManager::LoadScene(int sceneId)
	{
		const std::vector<std::string>& sceneInBuild = EditorApp::GetShippingSettings().GetScenesInBuild();
		
		if (sceneInBuild.size() > sceneId)
		{
			LoadScene(sceneInBuild[sceneId]);
		}
		else
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_WARNING, "No scene with id %d is set in the build settings", sceneId);
		}
	}

	void SceneManager::LoadScene(std::string_view filePath)
	{
		if (!filePath.empty() && std::filesystem::exists(filePath))
		{
			m_isSceneToLoad = true;
			m_sceneToLoadPath = filePath;
		}
		else
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_WARNING, "No file with the path %s exist", filePath);
		}
	}

	void SceneManager::LoadSceneToLoad()
	{
		if (m_isSceneToLoad)
		{
			LoadScene();
		}

		m_isSceneToLoad = false;
	}

	std::string SceneManager::GetActiveSceneName()
	{
		return m_currentScenePath;
	}

	void SceneManager::CreateDefaultScene(const std::string& openPath)
	{
		const std::string from = DEFAULT_RESOURCES_SCENE_PATH;
		const std::string to = openPath + "\\" + DEFAULT_SCENE_NAME;

		if (std::filesystem::exists(to))
		{
			return;
		}

		std::filesystem::create_directory(openPath);
		std::filesystem::copy(from, to);
	}

	void SceneManager::LoadScene()
	{
		Core::Serializer& serializer = EditorApp::GetSceneSerializer();

		if (!serializer.Deserialize(m_sceneToLoadPath.data()))
		{
			SetScenePath(std::string(DEFAULT_SCENE_FOLDER_PATH) + "\\" + DEFAULT_SCENE_NAME);
			CreateDefaultScene(DEFAULT_SCENE_FOLDER_PATH);
			serializer.Deserialize(m_currentScenePath.c_str());

		}
		else
		{
			SetScenePath(m_sceneToLoadPath);
		}

		std::pair<bool, Core::SaveElement> sceneElement = serializer.GetFirstElement(SCENE_ELEMENT);
		if (!sceneElement.first)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_WARNING, "Failed to find a scene element in the save");
		}

		//Load root elem
		std::pair<bool, Core::SaveElement> rootGameObjectElem = sceneElement.second.GetFirstChild();

		if (!rootGameObjectElem.first)
		{
			return;
		}

		if (m_activeScene == m_editorScene)
		{
			delete m_editorScene;
			m_editorScene = new Scene(new GameObject(rootGameObjectElem.second.GetStringAttribute("name")));
			m_editorScene->sceneName = m_sceneToLoadPath;
			SetActiveScene(m_editorScene);
			EditorApp::GetRenderingContext().GetCurrentCamera()->SetPosRot({0.0f, 0.0f, 0.0f}, { 0.0f, 0.0f, 1.0f });
			m_editorScene->sceneRoot->Deserialize(rootGameObjectElem.second);
		}
		else if (m_activeScene == m_simulationScene)
		{
			delete m_simulationScene;
			m_simulationScene = new Scene(new GameObject(rootGameObjectElem.second.GetStringAttribute("name")));
			m_simulationScene->sceneName = m_sceneToLoadPath;
			SetActiveScene(m_simulationScene);
			m_simulationScene->sceneRoot->Deserialize(rootGameObjectElem.second);

			if (!UIEditor::GetInspectorWindow().InitObjectsTagged())
			{
				LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "Could not init object tagged when loading a scene at runtime");
				return;
			}

			EditorApp::GetRenderingContext().SetCurrentCamera(m_mainCamera);
			
		}

		UIEditor::GetInspectorWindow().SetGameObject(nullptr);
	}
}
