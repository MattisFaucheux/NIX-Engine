#pragma once
#include <string_view>


#include "API/DLLExportSound.h"

namespace FMOD {
	class Channel;
	class Sound;
}

namespace NIX::Sound
{
	class Channel;
	class SoundSystem;

	class Sound
	{
	private:		
		FMOD::Sound* m_sound;

		std::string m_path = "";
		
	public:
		Sound() = delete;
		SOUND_LIBRARY_API Sound(std::string_view soundPath);
		~Sound();
		
		SOUND_LIBRARY_API void Play(Channel& channel);

		void SetMode(const Channel& channel);

		SOUND_LIBRARY_API std::string_view GetPath() const;
	};
}
