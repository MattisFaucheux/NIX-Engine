#pragma once

#include "Log/ILoggerClient.h"

namespace NIX::Editor
{
	class EditorConsoleLogger : public Core::ILoggerClient
	{
	public:
		void PrintLog(const Core::EVerbosity verbosity, const Core::SourceInfo& sourceInfo, const char* message) override;
	};
}
