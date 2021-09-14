#include "pch.h"
#include "Scripting/LuaForceMode.h"

#include "Log/Logger.h"

namespace NIX::Editor
{
	void LuaForceMode::Init()
	{
		m_enumMap["Force"] = Physic::EForceMode::FORCE;
		m_enumMap["Impulse"] = Physic::EForceMode::IMPULSE;
		m_enumMap["Velocity"] = Physic::EForceMode::VELOCITY;
		m_enumMap["Acceleration"] = Physic::EForceMode::ACCELERATION;

		m_enumMap["FORCE"] = Physic::EForceMode::FORCE;
		m_enumMap["IMPULSE"] = Physic::EForceMode::IMPULSE;
		m_enumMap["VELOCITY"] = Physic::EForceMode::VELOCITY;
		m_enumMap["ACCELERATION"] = Physic::EForceMode::ACCELERATION;
	}

	Physic::EForceMode LuaForceMode::GetForceMode(std::string foceModeString)
	{
		const std::unordered_map<std::string, Physic::EForceMode>::iterator it = m_enumMap.find(foceModeString);

		if (it == m_enumMap.end())
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_WARNING, "%s dosen't exist in ForceMode enum.", foceModeString.c_str());
			return Physic::EForceMode::FORCE;
		}

		return m_enumMap[foceModeString];
	}
}