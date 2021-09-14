#include "pch.h"

#include "Log/Logger.h"


#include <filesystem>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdio>

namespace NIX::Core
{
	SourceInfo::SourceInfo(const char* pFile, const int pLine)
		: file(pFile), line(pLine)
	{}

	Logger::Logger()
	{		
		if (m_instance != nullptr)
		{
			return;
		}
		
		m_instance = this;
	}

	Logger::~Logger()
	{
		for (ILoggerClient* client : m_clients)
		{
			delete client;
		}
	}

	Logger* Logger::GetLogger()
	{
		return m_instance;
	}

	void Logger::AddClient(ILoggerClient* client)
	{
		m_clients.push_back(client);
	}

	std::string Logger::VerbosityToString(EVerbosity verbosity)
	{
		switch (verbosity)
		{
		case EVerbosity::LOG_TRACE:
			return "TRACE ";
		case EVerbosity::LOG_DEBUG:
			return "DEBUG ";
		case EVerbosity::LOG_INFO:
			return "INFO ";
		case EVerbosity::LOG_WARNING:
			return "WARNING ";
		case EVerbosity::LOG_ERROR:
			return "ERROR ";
		case EVerbosity::LOG_FATAL:
			return "FATAL ";
		default:
			return "default case";
		}
		
	}

	void Logger::CheckNbOfFiles()
	{
		const auto dirIter = std::filesystem::directory_iterator(LOG_FOLDER);

		const int fileCount = static_cast<int>(std::count_if(begin(dirIter), end(dirIter), [](auto& entry) { return entry.is_regular_file(); }));

		if (fileCount > LOG_FILE_MAX_QUANTITY)
		{
			printf("Log file are never deleted, current count is : %d\n", fileCount);
			//TODO find a way to delete oldest file
		}
	}
}


