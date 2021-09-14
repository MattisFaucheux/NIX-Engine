#pragma once
#include <string>

namespace NIX::Editor
{
	class LuaLog
	{
	public:

		static void DebugLog(const std::string& s);

		static void LuaInit();
	};

}