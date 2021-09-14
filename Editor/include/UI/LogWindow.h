#pragma once
#include <vector>
#include <string>

#include "Vector/Vector4.h"

namespace NIX {
	namespace Core {
		struct SourceInfo;
		enum class EVerbosity;
	}
}

namespace NIX::Editor
{
	enum class ELogLevel
	{
		ERROR = 0,
		WARNING = 1,
		INFO = 2,
		DEBUG = 3,
	};

	class LogWindow
	{
		bool isCollapsed = false;

		std::vector<std::pair<std::string, std::string>> m_logsInfo;
		std::vector<std::pair<std::string, LibMath::Vector4>> m_logs;

		std::vector<int> m_logsStacked;
		std::vector<std::pair<std::string, std::string>> m_logsInfoDisplayed;
		std::vector<std::pair<std::string, LibMath::Vector4>> m_logsDisplayed;
		
	public:
		void Clear();
		void Send(Core::EVerbosity logLevel, const Core::SourceInfo& from, std::string_view log);
		void UpdateLogWindow();
	};

}
