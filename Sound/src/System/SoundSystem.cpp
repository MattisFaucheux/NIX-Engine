#include "pch.h"

#include "System/SoundSystem.h"
#include "Sound/Sound.h"

#include <fmod.hpp>
#include <fmod_errors.h>

#include "Log/Logger.h"

namespace NIX::Sound
{
	SoundSystem::SoundSystem()
	{		
		FMOD_RESULT result;

		result = System_Create(&m_system);
		if (result != FMOD_OK)
		{
			ASSERT(SOURCE_INFO, "FMOD error! (%d) %s\n", FMOD_ErrorString(result));
		}

		result = m_system->init(512, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
		if (result != FMOD_OK)
		{
			ASSERT(SOURCE_INFO, "FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		}

		m_soundSystem = this;
	}

	SoundSystem::~SoundSystem()
	{
		m_system->release();
	}

	Sound* SoundSystem::CreateSound(std::string_view soundPath)
	{
		return m_soundManager.Find(soundPath.data());
	}

	SoundSystem* SoundSystem::GetInstance()
	{
		return m_soundSystem;
	}

	SoundManager& SoundSystem::GetSoundManager()
	{
		return m_soundManager;
	}

	void SoundSystem::SetPause(bool value)
	{
		FMOD::ChannelGroup* masterChannel = nullptr;
		GetInstance()->m_system->getMasterChannelGroup(&masterChannel);

		masterChannel->setPaused(value);
	}

	FMOD::System* SoundSystem::GetHandle()
	{
		return m_system;
	}
}
