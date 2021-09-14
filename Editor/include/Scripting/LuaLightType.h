#pragma once
#include "Light/ELightType.h"
#include <string>
#include <unordered_map>

namespace NIX::Editor
{
	class LuaLightType
	{
	public:

		static void Init();

		static Rendering::ELightType GetLightType(std::string lightTypeString);


	private:

		inline static std::unordered_map<std::string, Rendering::ELightType> m_enumMap;
	};
}