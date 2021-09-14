#include "pch.h"

#include "Sound/Channel.h"

#include <fmod_errors.h>

#include "Log/Logger.h"
#include "System/SoundSystem.h"
#include "fmod.hpp"

namespace NIX::Sound
{
	FMOD::Channel** Channel::MutateChannel()
	{
		return &m_channel;
	}

	void Channel::Stop()
	{
		if (m_channel == nullptr)
		{
			return;
		}
		
		const FMOD_RESULT result =  m_channel->stop();
		if (result != FMOD_OK)
		{
			ASSERT(SOURCE_INFO, "FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		}
	}

	float Channel::GetVolume() const
	{
		return m_volume;
	}

	void Channel::SetVolume(float volume)
	{
		m_volume = volume;

		if (m_channel != nullptr)
		{
			m_channel->setVolume(m_volume);
		}
	}

	bool Channel::GetIsLooping() const
	{
		return m_isLooping;
	}

	void Channel::SetIsLooping(bool value)
	{
		m_isLooping = value;
	}

	bool Channel::GetIsPlayOnAwake() const
	{
		return m_isPlayOnAwake;
	}

	void Channel::SetIsPlayOnAwake(bool value)
	{
		m_isPlayOnAwake = value;
	}
}
