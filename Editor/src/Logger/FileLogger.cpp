#include "pch.h"

#include "FileLogger.h"

#include "Log/Logger.h"

namespace NIX::Editor
{
	FileLogger::FileLogger(const char* filePath)
		: m_filePath(filePath)
	{
		m_file.open(m_filePath);
	}

	FileLogger::~FileLogger()
	{
		m_file.close();
	}

	void FileLogger::PrintLog(const Core::EVerbosity verbosity, const Core::SourceInfo& sourceInfo, const char* message)
	{
		m_file << Core::Logger::VerbosityToString(verbosity)<< " " << sourceInfo.file << " (" << sourceInfo.line << ") : " << message << std::endl;
	}
}
