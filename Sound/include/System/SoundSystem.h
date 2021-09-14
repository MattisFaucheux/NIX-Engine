#pragma once

#include <string_view>

#include "API/DLLExportSound.h"
#include "Sound/SoundManager.h"

namespace FMOD {
	class System;
}

namespace NIX::Sound
{
	class Sound;
	
	class SoundSystem
	{
	private:
		inline static SoundSystem* m_soundSystem;
		
		FMOD::System* m_system;

		SoundManager m_soundManager;
		
	public:
		SOUND_LIBRARY_API SoundSystem();
		SOUND_LIBRARY_API ~SoundSystem();

		SOUND_LIBRARY_API Sound* CreateSound(std::string_view soundPath);
		
		SOUND_LIBRARY_API static SoundSystem* GetInstance();

		SOUND_LIBRARY_API SoundManager& GetSoundManager();

		SOUND_LIBRARY_API void SetPause(bool value);
		
		FMOD::System* GetHandle();
	};
}
