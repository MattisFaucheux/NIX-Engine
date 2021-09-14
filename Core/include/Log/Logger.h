#pragma once

#include "ILoggerClient.h"
#include "API/DLLExportCore.h"

#include <fstream>
#include <vector>

#define LOG_FOLDER "Log"
#define LOG_FILE_MAX_QUANTITY 5

namespace NIX::Core
{
	struct SourceInfo
	{
		CORE_LIBRARY_API SourceInfo(const char* file, const int line);

		const char* file = nullptr;
		const int line = 0;
	};
	
	enum class EVerbosity
	{
		LOG_TRACE = 0,
		LOG_DEBUG,
		LOG_INFO,
		LOG_WARNING,
		LOG_ERROR,
		LOG_FATAL
	};

	class Logger
	{
	public:
		
		CORE_LIBRARY_API Logger();
		Logger(const Logger& other) = delete;
		Logger(Logger& other) = delete;
		Logger(const Logger&& other) = delete;
		Logger(Logger&& other) = delete;
		CORE_LIBRARY_API ~Logger();
		
		CORE_LIBRARY_API  static  Logger* GetLogger();


		CORE_LIBRARY_API void AddClient(ILoggerClient* client);
		
		template<typename... Args>
		void PrintLog(const SourceInfo& sourceInfo, EVerbosity verbosity, const char* message, Args ... args) const
		{			
			char buffer[999];
			snprintf(buffer, 999, message, args...);

			for (ILoggerClient* client : m_clients)
			{
				client->PrintLog(verbosity, sourceInfo, buffer);
			}

			if (verbosity >= EVerbosity::LOG_FATAL)
			{
				exit(-1);
			}
		}
		
		template<typename... Args>
		void PrintAssert(const SourceInfo& sourceInfo, const char*  message, Args ... args) const
		{
			PrintLog(sourceInfo, EVerbosity::LOG_FATAL, message, args...);
		}
		
		CORE_LIBRARY_API static std::string VerbosityToString(EVerbosity verbosity);
		
	private:

		void CheckNbOfFiles();
		
		inline static Logger* m_instance = nullptr;

		std::vector<ILoggerClient*> m_clients;
		
	};

#define SOURCE_INFO NIX::Core::SourceInfo(__FILE__, __LINE__)
	
#define LOG(sourceInfo, verbosity, message, ...) NIX::Core::Logger::GetLogger()->PrintLog(sourceInfo, verbosity, message, __VA_ARGS__)
	
#define ASSERT(sourceInfo, message, ...) NIX::Core::Logger::GetLogger()->PrintAssert(sourceInfo, message, __VA_ARGS__)
	
}


