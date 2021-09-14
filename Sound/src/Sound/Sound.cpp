#include "pch.h"

#include "Sound/Sound.h"

#include <fmod_errors.h>



#include "Log/Logger.h"
#include "System/SoundSystem.h"

#include "fmod.hpp"
#include "Sound/Channel.h"

namespace NIX::Sound
{
	Sound::Sound(std::string_view soundPath)
		: m_path(soundPath)
	{
		FMOD::System* system = SoundSystem::GetInstance()->GetHandle();
		
		if (system == nullptr)
		{
			ASSERT(SOURCE_INFO, "Sound system is nullptr");
		}
				
		const FMOD_RESULT result = system->createSound(soundPath.data(), FMOD_DEFAULT, nullptr, &m_sound);
		if (result != FMOD_OK)
		{
			LOG(SOURCE_INFO,Core::EVerbosity::LOG_ERROR,  "FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		}
	}

	Sound::~Sound()
	{
		const FMOD_RESULT result = m_sound->release();

		if (result != FMOD_OK)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		}
	}

	void Sound::Play(Channel& channel)
	{
		if (!channel.GetIsPlayOnAwake())
		{
			return;
		}
		
		SetMode(channel);
		
		FMOD::System* system = SoundSystem::GetInstance()->GetHandle();
		
		if (system == nullptr)
		{
			ASSERT(SOURCE_INFO, "Sound system is nullptr");
		}

		const FMOD_RESULT result = system->playSound(m_sound, nullptr, false, channel.MutateChannel());
		if (result != FMOD_OK)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		}

	}

	void Sound::SetMode(const Channel& channel)
	{
		FMOD_MODE mode;

		if (channel.GetIsLooping())
		{
			mode = FMOD_LOOP_NORMAL;

			m_sound->setMode(mode);
			return;
		}
		mode = FMOD_DEFAULT;

		m_sound->setMode(mode);
	}

	std::string_view Sound::GetPath() const
	{
		return m_path;
	}
}
