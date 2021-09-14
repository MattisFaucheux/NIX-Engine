#include "Components/Sound/SoundEmitter.h"

#include "EditorApp.h"
#include "Log/Logger.h"
#include "System/SoundSystem.h"

#include "Scripting/ScriptManager.h"

namespace NIX::Editor
{
	SoundEmitter::SoundEmitter(GameObject* gameObject)
		: Component(gameObject)
	{}

	SoundEmitter::SoundEmitter(GameObject* gameObject, const SoundEmitter& other)
		: Component(gameObject)
	{
		m_sound = other.m_sound;
		m_volume = other.m_volume;
		m_isLooping = other.m_isLooping;
		m_isPlayingOnAwake = other.m_isPlayingOnAwake;

		m_channel = other.m_channel;
		

		SetIsActive(gameObject->IsActive());
	}

	SoundEmitter::~SoundEmitter()
	{
		Stop();
	}

	void SoundEmitter::Serialize(Core::SaveElement& parent)
	{
		Core::SaveElement soundElem(parent, SOUND_EMITTER_ELEMENT);

		soundElem.AddStringAttribute(SOUND_EMITTER_PATH_ATTRIBUTE, m_sound->GetPath().data());
		soundElem.AddFloatAttribute(SOUND_EMITTER_VOLUME, m_volume);
		soundElem.AddBoolAttribute(SOUND_EMITTER_IS_ACTIVE, m_isActive);
		soundElem.AddBoolAttribute(SOUND_EMITTER_IS_PLAY_ON_AWAKE, m_isPlayingOnAwake);
		soundElem.AddBoolAttribute(SOUND_EMITTER_IS_LOOPING, m_isLooping);
	}

	void SoundEmitter::Deserialize(const Core::SaveElement& data)
	{
		const char* soundPath = data.GetStringAttribute(SOUND_EMITTER_PATH_ATTRIBUTE);
		
		m_sound = EditorApp::GetSoundSystem().GetSoundManager().Find(soundPath);

		if (m_sound == nullptr)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_WARNING, "Failed to load sound : %s", soundPath);
		}

		m_volume = data.GetFloatAttribute(SOUND_EMITTER_VOLUME);
		m_isActive = data.GetBoolAttribute(SOUND_EMITTER_IS_ACTIVE);
		m_isPlayingOnAwake = data.GetBoolAttribute(SOUND_EMITTER_IS_PLAY_ON_AWAKE);
		m_isLooping = data.GetBoolAttribute(SOUND_EMITTER_IS_LOOPING);
		
	}

	void SoundEmitter::SetIsActive(bool value)
	{
		m_isActive = value;
	}

	void SoundEmitter::SetSound(Sound::Sound* sound)
	{
		m_channel.Stop();
		m_sound = sound;
	}

	Sound::Sound* SoundEmitter::GetSound() const
	{
		return m_sound;
	}

	std::string_view SoundEmitter::GetSoundPath() const
	{
		return m_sound->GetPath();
	}

	void SoundEmitter::Play()
	{
		m_sound->Play(m_channel);

		m_channel.SetVolume(m_volume);
	}

	void SoundEmitter::Stop()
	{
		m_channel.Stop();
	}

	float SoundEmitter::GetVolume() const
	{
		return m_volume;
	}

	void SoundEmitter::SetVolume(float volume)
	{
		m_volume = volume;
		m_channel.SetVolume(m_volume);
	}

	bool SoundEmitter::GetIsLooping() const
	{
		return m_isLooping;
	}

	void SoundEmitter::SetIsLooping(bool value)
	{
		m_isLooping = value;
		m_channel.SetIsLooping(value);
	}

	bool SoundEmitter::GetIsPlayingOnAwake() const
	{
		return m_isPlayingOnAwake;
	}

	void SoundEmitter::SetIsPlayingOnAwake(bool value)
	{
		m_isPlayingOnAwake = value;
		m_channel.SetIsPlayOnAwake(value);
	}

	void SoundEmitter::LuaInit()
	{
		ScriptManager::GetSingleton()->AddClass<SoundEmitter, GameObject*>("SoundEmitter");

		ScriptManager::GetSingleton()->AddClassProperty<SoundEmitter>("SoundEmitter", "IsActive", &SoundEmitter::GetIsActive, &SoundEmitter::SetIsActive);
		ScriptManager::GetSingleton()->AddClassData<SoundEmitter>("SoundEmitter", "GameObject", &SoundEmitter::m_gameObject, false);
		ScriptManager::GetSingleton()->AddClassFunction<SoundEmitter>("SoundEmitter", "GetGameObject", &SoundEmitter::GetParent);

		ScriptManager::GetSingleton()->AddClassFunction<SoundEmitter>("SoundEmitter", "SetVolume", &SoundEmitter::SetVolume);
		ScriptManager::GetSingleton()->AddClassProperty<SoundEmitter>("SoundEmitter", "IsPlayingOnAwake", &SoundEmitter::GetIsPlayingOnAwake, &SoundEmitter::SetIsPlayingOnAwake);
		ScriptManager::GetSingleton()->AddClassProperty<SoundEmitter>("SoundEmitter", "IsLooping", &SoundEmitter::GetIsLooping, &SoundEmitter::SetIsLooping);
		ScriptManager::GetSingleton()->AddClassFunction<SoundEmitter>("SoundEmitter", "Play", &SoundEmitter::Play);
		ScriptManager::GetSingleton()->AddClassFunction<SoundEmitter>("SoundEmitter", "Stop", &SoundEmitter::Stop);
		
	}
}
