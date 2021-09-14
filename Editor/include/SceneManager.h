#pragma once

#include "EditorApp.h"
#include "GameObject.h"
#include "PhysicScene.h"

#define SCENE_ELEMENT "scene"

#define DEFAULT_SCENE_FOLDER_PATH "Assets\\Scenes"
#define DEFAULT_SCENE_NAME "defaultScene.scene"
#define DEFAULT_RESOURCES_SCENE_PATH "Resources\\Scenes\\defaultScene.scene"

namespace NIX::Editor
{
	class Camera;

	struct Scene
	{
		GameObject* sceneRoot = nullptr;
		Physic::PhysicScene* physicSimulation = nullptr;
		std::string sceneName = "";

		Scene() = default;
		
		Scene(GameObject* root)
		{
			physicSimulation = new Physic::PhysicScene();
			Physic::PhysicSimulation::GetSingleton()->SetCurrentScene(physicSimulation);
			sceneRoot = new GameObject(nullptr, *root);
			
		}

		~Scene();

		void IsActive(bool value);
		void Update();

		void SwitchToSimulation();
		void ExitSimulation();
		
		void SimulationUpdate(float deltaTime);
		void SimulationStart();

		void UpdateWorldMatrix();

		void Serialize(Core::Serializer& serializer, const std::string& scenePath);

	};
	
	class SceneManager
	{
	private:
		inline static Scene* m_editorScene;
		inline static Scene* m_simulationScene = nullptr;
		inline static Scene* m_activeScene = nullptr;

		inline static std::string m_currentScenePath = "";

		inline static Rendering::Camera* m_editorCamera;
		inline static Rendering::Camera* m_mainCamera;

		inline static bool m_isSceneToLoad = false;
		inline static std::string m_sceneToLoadPath = "";

	public:

		SceneManager();

		static void Init();
		
		static void InitCamera();
		[[nodiscard]] static Rendering::Camera* GetEditorCamera(); 

		static void SetScenePath(std::string_view scene);
		[[nodiscard]] static std::string_view GetScenePath();

		static void SetMainCamera(Rendering::Camera* camera);
		[[nodiscard]] static Rendering::Camera* GetMainCamera(); 
		
		static void SetActiveScene(Scene* activeScene);
		[[nodiscard]] static Scene* GetActiveScene();
		[[nodiscard]] static Scene* GetEditorScene();
		[[nodiscard]] static Scene* GetSimulationScene();

		static void InitSimulation();
		static void DestroySimulation();

		static void LoadScene(int sceneId);
		static void LoadScene(std::string_view filePath);

		
		static void LoadSceneToLoad();

		static std::string GetActiveSceneName();

		static void CreateDefaultScene(const std::string& openPath);

	private:
		static void LoadScene();
	};
}
