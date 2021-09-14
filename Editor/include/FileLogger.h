#pragma once

#include <fstream>

#include "Log/ILoggerClient.h"

namespace NIX::Editor
{
	class FileLogger : public Core::ILoggerClient
	{
	public:
		FileLogger(const char* filePath);
		~FileLogger();
		void PrintLog(const Core::EVerbosity verbosity, const Core::SourceInfo& sourceInfo, const char* message) override;
	private:
		const char* m_filePath;
		std::ofstream m_file;
	};
}
