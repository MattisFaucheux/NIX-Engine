#pragma once

#include "Log/ILoggerClient.h"

namespace NIX::Editor
{
	class ConsoleLogger : public Core::ILoggerClient
	{
	public:
		ConsoleLogger() = default;
		
		void PrintLog(const Core::EVerbosity verbosity, const Core::SourceInfo& sourceInfo, const char* message) override;
	};
}
