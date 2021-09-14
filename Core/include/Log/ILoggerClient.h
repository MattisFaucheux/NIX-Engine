#pragma once

namespace NIX::Core
{
	struct SourceInfo;
	enum class EVerbosity;

	class ILoggerClient
	{

	public:
		virtual ~ILoggerClient() = default;
		virtual void PrintLog(const EVerbosity verbosity, const SourceInfo& sourceInfo, const char* message) = 0;
		
	};
}
