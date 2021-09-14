#pragma once
#include "ForceMode.h"
#include <String>
#include <unordered_map>

namespace NIX::Editor
{
	class LuaForceMode
	{
	public:

		static void Init();

		static Physic::EForceMode GetForceMode(std::string foceModeString);


	private:

		inline static std::unordered_map<std::string, Physic::EForceMode> m_enumMap;
	};

}
