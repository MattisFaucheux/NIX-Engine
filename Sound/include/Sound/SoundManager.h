#pragma once
#include <vector>

#include "Sound.h"
#include "ResourceManager/TResourceManager.h"

#include "API/DLLExportSound.h"

namespace NIX::Sound
{
	class SoundManager : public Core::TResourceManager<Sound*>
	{
	private:
		
	public:
		SOUND_LIBRARY_API SoundManager() = default;
		
		std::pair<bool, Sound*> LoadFile(const char* filename) override;

		void Clear() override;
	};
}
