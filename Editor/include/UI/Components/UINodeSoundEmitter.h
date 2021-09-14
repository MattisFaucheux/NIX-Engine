#pragma once
#include <string>

#include "UINodeComponent.h"

namespace std {
	namespace filesystem {
		class path;
	}
}

namespace NIX::Editor
{
	class SoundEmitter;
	class GameObject;

	class UINodeSoundEmitter : public UINodeComponent
	{
	private:		
		std::string m_soundPath = "";
		std::string m_soundDrag = "";
		float m_volume = 1.0f;
		bool m_isLooping = true;
		bool m_isPlayingOnAwake = true;
	public:		
		void UpdateComponentProperties(GameObject* obj) override;

		void UpdateComponent() override;

		void SetSoundDrag(std::filesystem::path path);

	private:
		void UpdateComponentNode(bool& isComponentOpen) override;

		void UpdateSoundPath(SoundEmitter* soundEmitter);
		void UpdateIsLooping(SoundEmitter* soundEmitter);
		void UpdateIsPlayingOnAwake(SoundEmitter* soundEmitter);
		void UpdateVolume(SoundEmitter* soundEmitter);

	};


}
