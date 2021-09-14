#include "pch.h"

#include "Scripting\LuaLog.h"

#include "Log/Logger.h"
#include "Scripting/ScriptManager.h"

namespace NIX::Editor
{
	void LuaLog::DebugLog(const std::string& s)
	{
		LOG(SOURCE_INFO, Core::EVerbosity::LOG_DEBUG, s.c_str());
	}

	void LuaLog::LuaInit()
	{
		ScriptManager::GetSingleton()->AddFunctionInNamespace("Log", "DebugLog", LuaLog::DebugLog);
	}
}
