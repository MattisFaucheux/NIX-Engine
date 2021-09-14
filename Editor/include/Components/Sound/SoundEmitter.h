#pragma once

#include <Component.h>
#include "Sound/Sound.h"
#include "Sound/Channel.h"

#define SOUND_EMITTER_ELEMENT "sound_emitter"
#define SOUND_EMITTER_PATH_ATTRIBUTE "sound_path"
#define SOUND_EMITTER_VOLUME "volume"
#define SOUND_EMITTER_IS_ACTIVE "is_active"
#define SOUND_EMITTER_IS_PLAY_ON_AWAKE "is_play_on_awake"
#define SOUND_EMITTER_IS_LOOPING "is_looping"

namespace NIX::Editor
{
	class SoundEmitter : public Component
	{
	private:
		Sound::Sound* m_sound = nullptr;
		Sound::Channel m_channel;

		float m_volume = 1.0f;
		bool m_isLooping = true;
		bool m_isPlayingOnAwake = true;

	public:
		SoundEmitter() = delete;
		SoundEmitter(GameObject* gameObject);
		SoundEmitter(GameObject* gameObject, const SoundEmitter& other);
		~SoundEmitter();

		void Serialize(Core::SaveElement& parent) override;

		void Deserialize(const Core::SaveElement& data) override;

		void SetIsActive(bool value) override;

		void SetSound(Sound::Sound* sound);

		[[nodiscard]] Sound::Sound* GetSound() const;
		[[nodiscard]] std::string_view GetSoundPath() const;
		
		void Play();
		void Stop();

		[[nodiscard]] float GetVolume() const;
		void SetVolume(float volume);
		
		[[nodiscard]] bool GetIsLooping() const;
		void SetIsLooping(bool value);

		[[nodiscard]] bool GetIsPlayingOnAwake() const;
		void SetIsPlayingOnAwake(bool value);

		static void LuaInit();
		
	};
}
