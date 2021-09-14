#include "pch.h"

#include "Sound/SoundManager.h"

namespace NIX::Sound
{
	std::pair<bool, Sound*> SoundManager::LoadFile(const char* filename)
	{
		Sound* sound = new Sound(filename);

		if (sound == nullptr)
		{
			return { false, nullptr };
		}
		
		return { true, sound };
	}

	void SoundManager::Clear()
	{
		for (auto resource : m_resourceStore)
		{
			delete resource.second;
		}
	}
}
