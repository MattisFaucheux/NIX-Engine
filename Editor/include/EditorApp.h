#pragma once

#include "ResourceManager/MainResourceManager.h"
#include "System/SoundSystem.h"
#include "Context/Context.h"
#include "PhysicSimulation.h"
#include "Windowing/Window.h"
#include "Windowing/InputManager.h"
#include "DrawableObject/DrawableObject.h"
#include "GameObject.h"
#include "EngineState.h"
#include "ProjectSave.h"
#include "Components/Camera.h"
#include "Shipping/ShippingSettings.h"
#include "Scripting/ScriptManager.h"

#include "Sound/SoundManager.h"

namespace NIX {
	namespace Rendering {
		enum class ELightType;
	}

	namespace Sound
	{
		class Sound;
	}
}

namespace NIX::Editor
{
	class UIEditor;

	class EditorApp
	{
		public:

			EditorApp();

			~EditorApp();
			void Run();

			float UpdateDeltaTime();

			void UpdateCameraInput(float deltaTime);
			void UpdateSpeedCamera(double xOffSet, double yOffset);
		
			//void ChangeCamera(Rendering::EInputKeyboard inputKeyboard, Rendering::EInputState inputState);
			

			static bool SwitchEngineState();
			static bool SwitchPauseMode();
			static bool SwitchNextFrameMode();
		
			static void SaveScene();

			static Rendering::Context& GetRenderingContext();
			static Core::Serializer& GetSceneSerializer();
			static ShippingSettings& GetShippingSettings();
			static Sound::SoundSystem& GetSoundSystem();

			static EngineState GetActualState();


		
		private:		
			static bool SwitchPlayMode();
			void PlayModeUpdate(float deltaTime);

			static void SwitchEditorMode();
			void EditorModeUpdate(float deltaTime);

			void PauseModeUpdate(float deltaTime);

			void NextFrameModeUpdate(float deltaTime);
		
			void UpdateWindow(Rendering::Context& pRenderingContext);

			void UpdatePhysicsDebugDraw();
			void SetupDebugDrawObject();

			Rendering::Window window;
			inline static MainResourceManager m_resourceManager;
			Rendering::InputManager m_inputManager;
			UIEditor* m_uiEditor = nullptr;
			
			inline static Rendering::Context* renderingContext = nullptr;

			Physic::PhysicSimulation physicSimulation;
			Rendering::DrawableObject* m_physicsDebugObject = nullptr;
			Rendering::DrawableObject* m_physicsDebugObjectSimulation = nullptr;
			inline static bool m_isSimulationRunning = false;

			float m_totalTime = 0.f;

			ScriptManager scriptManager;

			inline static EngineState m_actualState = EngineState::EDITOR;

			inline static Core::Serializer m_serializer;

			inline static ShippingSettings m_shippingSettings;
			inline static ProjectSave m_projectSave;

			Camera * m_sceneCamera;

			inline static Sound::SoundSystem m_soundSystem;

			int m_frameCounter = 0;
			float m_accumulateTime = 0.0f;
			void UpdateFps(float deltaTime);
	};
}
