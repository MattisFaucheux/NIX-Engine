#pragma once

#include "API/DLLExportSound.h"

namespace FMOD {
	class Channel;
}

namespace NIX::Sound
{
	class Sound;
	
	class Channel
	{
	private:
		FMOD::Channel* m_channel;

		float m_volume = 1.0f;
		bool m_isLooping = true;
		bool m_isPlayOnAwake = true;
		
	public:
		Channel() = default;
		~Channel() = default;

		FMOD::Channel** MutateChannel();

		SOUND_LIBRARY_API void Stop();

		SOUND_LIBRARY_API [[nodiscard]] float GetVolume() const;
		SOUND_LIBRARY_API void SetVolume(float volume);
		
		SOUND_LIBRARY_API [[nodiscard]] bool GetIsLooping() const;
		SOUND_LIBRARY_API void SetIsLooping(bool value);

		SOUND_LIBRARY_API [[nodiscard]] bool GetIsPlayOnAwake() const;
		SOUND_LIBRARY_API void SetIsPlayOnAwake(bool value);

		

	};
}
