#include "EditorApp.h"
#include "Matrix/Matrix4.h"
#include "Material/Material.h"
#include "ResourceManager/TextureViewManager.h"
#include "ResourceManager/MaterialManager.h"
#include "ResourceManager/MeshManager.h"
#include "Camera/CameraManager.h"
#include "Context/Context.h"

#include "Vector/Vector3.h"

#include "RigidBody.h"
#include "Raycast.h"

#include "SceneManager.h"

#include <UI/UI.h>
#include "UI/Toolbar.h"
#include "UI/SceneWindow.h"
#include "UI/AssetsWindow.h"
#include "UI/ContentsWindow.h"
#include "UI/InspectorWindow.h"
#include "UI/LogWindow.h"
#include "Components/Model.h"
#include "Components/Lights/Light.h"
#include "Components/CollisionBox.h"
#include "Components/CollisionSphere.h"
#include "Components/CollisionMesh.h"
#include "Components/RigidBody.h"
#include "Components/RigidStatic.h"
#include "Components/Scripting/Script.h"

#include "Light/ELightType.h"
#include "Log/Logger.h"

#include "FileLogger.h"

#include "Serialization/AssetsParsing/JSonParser.h"
#include "Serialization/AssetsParsing/ParseMaterial.h"

#include "System/SoundSystem.h"
#include "Sound/SoundManager.h"
#include "Sound/Channel.h"
#include "Sound/Sound.h"

#include "UI/UIEditor.h"

namespace NIX::Editor
{
	EditorApp::EditorApp() :
		window("Nix Engine", 1280, 720, "Resources\\Images\\ApplicationIcon\\Logo.png"), physicSimulation(true)
	{
		renderingContext = new Rendering::Context(window, "Nix Engine", { 1, 0, 0 }, "Nix Engine", { 1, 0, 0 }, window.GetRequiredExtensions());
		m_uiEditor = new UIEditor();
		
		m_projectSave.Deserialize();

		physicSimulation.CreateMaterial(0.5f, 0.5f, 0.1f);

		MainResourceManager::GetMaterialManager()->LoadAllMaterials();

		SceneManager::Init();

		
		Rendering::InputManager::GetSingleton()->SetupInputsCallback(window);

		Rendering::InputManager::GetSingleton()->BindMouseScrollCallback(this, &EditorApp::UpdateSpeedCamera);
		
		SetupDebugDrawObject();

	}

	EditorApp::~EditorApp()
	{
		delete m_uiEditor;
	}

	void EditorApp::Run()
	{		
		physicSimulation.UpdateSimulation(1.f/60.f);
		
	#ifdef _SHIPPING
		SwitchPlayMode();
	#endif
		
		while (window.GetIsRunning())
		{
			SceneManager::LoadSceneToLoad();
			
			const float deltaTime = UpdateDeltaTime();
			UpdateFps(deltaTime);

			window.ReceiveInput();

	#ifndef _SHIPPING
			switch (m_actualState)
			{
				case EngineState::EDITOR:
				{
					EditorModeUpdate(deltaTime);
				}
				break;

				case EngineState::PLAY:
				{
					PlayModeUpdate(deltaTime);
				}
				break;

				case EngineState::PAUSE:
				{
					PauseModeUpdate(deltaTime);
				}
				break;

				case EngineState::NEXT_FRAME:
				{
					PlayModeUpdate(1.0f / 60.0f);
				}
				break;
			}
			
			UpdatePhysicsDebugDraw();

	#endif

	#ifdef _SHIPPING
			PlayModeUpdate(deltaTime);
	#endif
			if (window.GetWindowWidth() > 0 || window.GetWindowHeight() > 0)
			{
				UpdateWindow(*renderingContext);
				//if scene change continue
				
				renderingContext->DrawFrame();
			}
			
		}

		m_projectSave.Serialize();
		
		renderingContext->Shutdown();
		MainResourceManager::GetMaterialManager()->Clear();
		MainResourceManager::GetTextureViewManager()->Clear();
			
		delete renderingContext;
	}


	bool EditorApp::SwitchPlayMode()
	{	
		m_actualState = EngineState::PLAY;
		UIEditor::GetInspectorWindow().ClearObjectsTagged();

		SceneManager::InitSimulation();
		
		if (SceneManager::GetActiveScene()->sceneRoot == nullptr)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_WARNING, "Root of simulation scene is nullptr");
			return false;
		}
		
		UIEditor::GetInspectorWindow().SetGameObject(nullptr);

		m_isSimulationRunning = true;
		
		SceneManager::GetActiveScene()->Update();

		if (!UIEditor::GetInspectorWindow().InitObjectsTagged())
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "Couldn't launch Play Mode, swap to Editor Mode.");
			SwitchEditorMode();
			return false;
		}

		GetRenderingContext().SetCurrentCamera(SceneManager::GetMainCamera());

		SceneManager::GetSimulationScene()->SimulationStart();

		return true;
	}


	void EditorApp::PlayModeUpdate(float deltaTime)
	{
		if (SceneManager::GetSimulationScene() == nullptr)
		{
			return;
		}

		SceneManager::GetSimulationScene()->Update();

		physicSimulation.UpdateSimulation(deltaTime);

		SceneManager::GetSimulationScene()->SimulationUpdate(deltaTime);

		m_soundSystem.SetPause(false);
	}

	void EditorApp::SwitchEditorMode()
	{
		m_actualState = EngineState::EDITOR;

		GetRenderingContext().SetCurrentCamera(SceneManager::GetEditorCamera());

		UIEditor::GetInspectorWindow().SetGameObject(nullptr);

		SceneManager::DestroySimulation();
		
		m_isSimulationRunning = false;

		Physic::PhysicSimulation::GetSingleton()->UpdateSimulation(1.f / 60.f);
	}

	void EditorApp::EditorModeUpdate(float deltaTime)
	{
		//Update Inputs
		//window.ReceiveInput();
		UpdateCameraInput(deltaTime);
	}

	bool EditorApp::SwitchPauseMode()
	{	
		if (m_actualState == EngineState::PLAY || m_actualState == EngineState::NEXT_FRAME)
		{
			GetRenderingContext().SetCurrentCamera(SceneManager::GetEditorCamera());
			m_soundSystem.SetPause(true);
			m_actualState = EngineState::PAUSE;
			return true;
		}
		else if (m_actualState == EngineState::PAUSE)
		{
			GetRenderingContext().SetCurrentCamera(SceneManager::GetMainCamera());
			m_actualState = EngineState::PLAY;
		}

		return false;
	}

	bool EditorApp::SwitchNextFrameMode()
	{
		if (m_actualState == EngineState::PAUSE || m_actualState == EngineState::PLAY)
		{
			m_actualState = EngineState::NEXT_FRAME;
			m_soundSystem.SetPause(false);
			return true;
		}

		return false;
	}

	NIX::Rendering::Context& EditorApp::GetRenderingContext()
	{
		return *renderingContext;
	}

	NIX::Core::Serializer& EditorApp::GetSceneSerializer()
	{
		return m_serializer;
	}

	ShippingSettings& EditorApp::GetShippingSettings()
	{
		return m_shippingSettings;
	}

	NIX::Sound::SoundSystem& EditorApp::GetSoundSystem()
	{
		return m_soundSystem;
	}

	EngineState EditorApp::GetActualState()
	{
		return m_actualState;
	}


	void EditorApp::PauseModeUpdate(float deltaTime)
	{
		UpdateCameraInput(deltaTime);
	}

	void EditorApp::NextFrameModeUpdate(float deltaTime)
	{
		PlayModeUpdate(deltaTime);
	}

	bool EditorApp::SwitchEngineState()
	{
		if (m_actualState == EngineState::EDITOR)
		{
			return SwitchPlayMode();
		}
		else if (m_actualState == EngineState::PLAY || m_actualState == EngineState::PAUSE)
		{
			SwitchEditorMode();
		}
		return false;
	}

	float EditorApp::UpdateDeltaTime()
	{
		float actualTotalTime = Rendering::Window::GetTimeGlfw();
		
		float deltaTime = actualTotalTime - m_totalTime;
		m_totalTime = actualTotalTime;

		return deltaTime;
	}

	void EditorApp::UpdateCameraInput(float deltaTime)
	{
		if (Rendering::InputManager::GetSingleton()->GetInputState(Rendering::EInputMouse::BUTTON_RIGHT) == Rendering::EInputState::PRESS || Rendering::InputManager::GetSingleton()->GetInputState(Rendering::EInputMouse::BUTTON_RIGHT) == Rendering::EInputState::REPEAT)
		{
			if (Rendering::InputManager::GetSingleton()->IsInputRepeated(Rendering::EInputKeyboard::W) || Rendering::InputManager::GetSingleton()->IsInputPressed(Rendering::EInputKeyboard::W))
			{
				renderingContext->GetCurrentCamera()->MoveCameraForward(deltaTime);
			}
			if (Rendering::InputManager::GetSingleton()->IsInputRepeated(Rendering::EInputKeyboard::S) || Rendering::InputManager::GetSingleton()->IsInputPressed(Rendering::EInputKeyboard::S))
			{
				renderingContext->GetCurrentCamera()->MoveCameraBackward(deltaTime);
			}
			if (Rendering::InputManager::GetSingleton()->IsInputRepeated(Rendering::EInputKeyboard::A) || Rendering::InputManager::GetSingleton()->IsInputPressed(Rendering::EInputKeyboard::A))
			{
				renderingContext->GetCurrentCamera()->MoveCameraLeft(deltaTime);
			}
			if (Rendering::InputManager::GetSingleton()->IsInputRepeated(Rendering::EInputKeyboard::D) || Rendering::InputManager::GetSingleton()->IsInputPressed(Rendering::EInputKeyboard::D))
			{
				renderingContext->GetCurrentCamera()->MoveCameraRight(deltaTime);
			}
			
			renderingContext->GetCurrentCamera()->UpdateDirection((float)Rendering::InputManager::GetSingleton()->GetCursorPosX(), (float)Rendering::InputManager::GetSingleton()->GetCursorPosY());
		}
		
		if (Rendering::InputManager::GetSingleton()->GetInputState(Rendering::EInputMouse::BUTTON_RIGHT) == Rendering::EInputState::RELEASE)
		{
			renderingContext->GetCurrentCamera()->SetFirstMouse(true);
		}
	}

	void EditorApp::UpdateSpeedCamera(double /*xOffSet*/, double yOffset)
	{
		if (Rendering::InputManager::GetSingleton()->GetInputState(Rendering::EInputMouse::BUTTON_RIGHT) == Rendering::EInputState::PRESS || Rendering::InputManager::GetSingleton()->GetInputState(Rendering::EInputMouse::BUTTON_RIGHT) == Rendering::EInputState::REPEAT)
		{
			Rendering::Camera* cam = renderingContext->GetCurrentCamera();
			float speed = cam->GetCameraSpeed() + (float)yOffset;
			if (speed > 0.f)
			{
				cam->SetSpeed(speed);
			}
		}
	}

	void EditorApp::UpdateWindow(Rendering::Context& pRenderingContext)
	{
		pRenderingContext.StartFrame();
		
		
		UI::GetUI()->GlobalDockSpace();
		
	#ifndef _SHIPPING	
		UIEditor::GetToolbar().UpdateToolbar();
		UIEditor::GetLogWindow().UpdateLogWindow();
		UIEditor::GetAssetsWindow().UpdateAssetsWindow();
		UIEditor::GetContentsWindow().UpdateContentsWindow();
		UIEditor::GetInspectorWindow().UpdateInspectorWindow();

	#endif

		const std::pair<float, float> sceneSize = UIEditor::GetSceneWindow().UpdateSceneWindow(pRenderingContext.GetFrameImGuiID());

		pRenderingContext.UpdateOffscreenRenderSceneWindowSize(sceneSize.first, sceneSize.second);
	}


	void EditorApp::UpdatePhysicsDebugDraw()
	{
		const std::vector<LibMath::Vector3>& debugPoints = physicSimulation.GetDebugLines();

		Rendering::DrawableObject* debugDrawObject = nullptr;
		
		if (m_isSimulationRunning)
		{
			debugDrawObject = m_physicsDebugObjectSimulation;
			m_physicsDebugObjectSimulation->SetIsActive(true);
			m_physicsDebugObject->SetIsActive(false);
		}
		else
		{
			debugDrawObject = m_physicsDebugObject;
			m_physicsDebugObject->SetIsActive(true);
			m_physicsDebugObjectSimulation->SetIsActive(false);
		}
		
		if (debugPoints.empty())
		{
			if (debugDrawObject != nullptr)
			{
				debugDrawObject->ChangeVertex({ {0.0f, 0.0f, 0.0f} });
			}
		}
		else
		{
			debugDrawObject->ChangeVertex(debugPoints);
		}
	}

	void EditorApp::SetupDebugDrawObject()
	{
		m_physicsDebugObject = renderingContext->AddDebugDrawableObject({ {0.0f, 0.0f, 0.0f} });
		m_physicsDebugObjectSimulation = renderingContext->AddDebugDrawableObject({ {0.0f, 0.0f, 0.0f} });
		m_physicsDebugObjectSimulation->SetIsActive(false);
	}

	void EditorApp::UpdateFps(float deltaTime) 
	{
		m_accumulateTime += deltaTime;
		m_frameCounter++;

		if (m_accumulateTime >= 1.0f)
		{
			const int fps = static_cast<int>((m_frameCounter/ m_accumulateTime));
			m_accumulateTime = 0.f;
			m_frameCounter = 0;
			const std::string windowName = "Nix Engine - " + std::to_string(fps) + " FPS";
			window.SetName(windowName.c_str());
		}
	}

	void EditorApp::SaveScene()
	{
		SceneManager::GetEditorScene()->Serialize(m_serializer, SceneManager::GetScenePath().data());

		m_projectSave.Serialize();
	}

}