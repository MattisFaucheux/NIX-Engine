#include "UI/Components/UINodeSoundEmitter.h"


#include "EditorApp.h"
#include "Components/Sound/SoundEmitter.h"
#include "UI/UI.h"

#include "GameObject.h"
#include "UI/UIEditor.h"

#include <filesystem>

namespace NIX::Editor
{
	void UINodeSoundEmitter::UpdateComponentProperties(GameObject* obj)
	{
		if (obj == nullptr)
		{
			return;
		}

		m_obj = obj;

		SoundEmitter* soundEmitterComp = m_obj->GetComponent<SoundEmitter>();
		if (soundEmitterComp == nullptr || soundEmitterComp->GetSound() == nullptr)
		{
			m_soundPath = "";
			return;
		}

		m_soundPath = soundEmitterComp->GetSoundPath();
		m_isLooping = soundEmitterComp->GetIsLooping();
		m_isPlayingOnAwake = soundEmitterComp->GetIsPlayingOnAwake();
	}

	void UINodeSoundEmitter::UpdateComponent()
	{
		if (m_obj == nullptr || m_obj->GetComponent<SoundEmitter>() == nullptr)
		{
			return;
		}

		static bool isModelOpen = true;

		UI::GetUI()->NewChild("Sound Emitter", 0.0f, isModelOpen ? 130.0f : 30.0f, true);
		UI::GetUI()->SetNextItemOpen(isModelOpen);

		if (UI::GetUI()->AddNode("Sound Emitter"))
		{
			if (UIEditor::GetInspectorWindow().RightClickComp(m_obj->GetComponent<SoundEmitter>()))
			{
				UpdateComponentNode(isModelOpen);
			}
				
			UI::GetUI()->CloseNode();
		}
		else
		{
			UIEditor::GetInspectorWindow().RightClickComp(m_obj->GetComponent<SoundEmitter>());
			isModelOpen = false;
		}

		UI::GetUI()->EndChild();
	}

	void UINodeSoundEmitter::SetSoundDrag(std::filesystem::path path)
	{
		m_soundDrag = path.string();
	}

	void UINodeSoundEmitter::UpdateComponentNode(bool& isComponentOpen)
	{
		SoundEmitter* soundEmitterComp = m_obj->GetComponent<SoundEmitter>();
		if (soundEmitterComp == nullptr)
		{
			return;
		} 

		if (!UIEditor::GetInspectorWindow().OpenComponentNode(isComponentOpen, "##SoundEmitterTable"))
		{
			return;
		}
		
		UpdateComponentProperties(m_obj);
		
		UpdateSoundPath(soundEmitterComp);
		UpdateVolume(soundEmitterComp);
		UpdateIsLooping(soundEmitterComp);
		UpdateIsPlayingOnAwake(soundEmitterComp);

		UIEditor::GetInspectorWindow().CloseComponentNode();
	}


	void UINodeSoundEmitter::UpdateSoundPath(SoundEmitter* soundEmitter)
	{
		if (soundEmitter == nullptr)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_WARNING, "Sound emitter is null");
			return;
		}
		
		UIEditor::GetInspectorWindow().SetLeftText("Sound clip");
		UI::GetUI()->AddInputText("SoundPath", "Drop sound clip here", m_soundPath, true);
		if (UI::GetUI()->DragDropTarget("DragDropSounds"))
		{
			m_soundPath = m_soundDrag;

			Sound::Sound* newSound = EditorApp::GetSoundSystem().GetSoundManager().Find(m_soundPath.c_str());

			if (newSound != nullptr)
			{
				soundEmitter->SetSound(newSound);
			}
		}
	}

	void UINodeSoundEmitter::UpdateIsLooping(SoundEmitter* soundEmitter)
	{
		if (soundEmitter == nullptr)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_WARNING, "Sound emitter is null");
			return;
		}

		UIEditor::GetInspectorWindow().SetLeftText("Is Looping");

		if (UI::GetUI()->AddCheckbox("##SoundIsLooping", m_isLooping))
		{
			soundEmitter->SetIsLooping(m_isLooping);
		}
	}

	void UINodeSoundEmitter::UpdateIsPlayingOnAwake(SoundEmitter* soundEmitter)
	{
		if (soundEmitter == nullptr)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_WARNING, "Sound emitter is null");
			return;
		}

		UIEditor::GetInspectorWindow().SetLeftText("Play on awake");

		if (UI::GetUI()->AddCheckbox("##SoundIsPlayingOnAwake", m_isPlayingOnAwake))
		{
			soundEmitter->SetIsPlayingOnAwake(m_isPlayingOnAwake);
		}
	}

	void UINodeSoundEmitter::UpdateVolume(SoundEmitter* soundEmitter)
	{
		if (soundEmitter == nullptr)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_WARNING, "Sound emitter is null");
			return;
		}

		UIEditor::GetInspectorWindow().SetLeftText("Volume");

		if (UI::GetUI()->AddFloat("##SoundVolume", m_volume))
		{
			soundEmitter->SetVolume(m_volume);
		}
	}
}
