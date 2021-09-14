#include "pch.h"

#include "Logger/ConsoleLogger.h"
#include "Log/Logger.h"

#include <iostream>

namespace NIX::Editor
{
	void ConsoleLogger::PrintLog(const Core::EVerbosity verbosity, const Core::SourceInfo& sourceInfo, const char* message)
	{
		std::cout << Core::Logger::VerbosityToString(verbosity) << " " << sourceInfo.file << " (" << sourceInfo.line << ") : " << message << std::endl;
	}
}
