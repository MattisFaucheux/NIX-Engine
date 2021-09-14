#include "pch.h"
#include "Scripting/LuaLightType.h"

#include "Log/Logger.h"

namespace NIX::Editor
{
	void LuaLightType::Init()
	{
		m_enumMap["UNDEFINED"] = Rendering::ELightType::UNDEFINED;
		m_enumMap["DIRECTIONAL"] = Rendering::ELightType::DIRECTIONAL;
		m_enumMap["POINT"] = Rendering::ELightType::POINT;
		m_enumMap["SPOT"] = Rendering::ELightType::SPOT;

		m_enumMap["Undefined"] = Rendering::ELightType::UNDEFINED;
		m_enumMap["Directional"] = Rendering::ELightType::DIRECTIONAL;
		m_enumMap["Point"] = Rendering::ELightType::POINT;
		m_enumMap["Spot"] = Rendering::ELightType::SPOT;


	}

	Rendering::ELightType LuaLightType::GetLightType(std::string lightTypeString)
	{
		const std::unordered_map<std::string, Rendering::ELightType>::iterator it = m_enumMap.find(lightTypeString);

		if (it == m_enumMap.end())
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_WARNING, "%s dosen't exist in LightType enum.", lightTypeString.c_str());
			return Rendering::ELightType::UNDEFINED;
		}

		return m_enumMap[lightTypeString];
	}
}