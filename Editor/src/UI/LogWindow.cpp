#include "pch.h"
#include "UI/LogWindow.h"

#include "Log/Logger.h"

#include "UI/UI.h"

#include<ctime>

void NIX::Editor::LogWindow::Clear()
{
	m_logs.clear();
	m_logsInfo.clear();
	m_logsStacked.clear();
	m_logsDisplayed.clear();
	m_logsInfoDisplayed.clear();
}

void NIX::Editor::LogWindow::Send(Core::EVerbosity logLevel, const Core::SourceInfo& from, std::string_view log)
{
	time_t t = time(NULL);
	struct tm time;
	localtime_s(&time, &t);

	std::string str = "[";
	str += (time.tm_hour < 10 ? "0" : ""); str += std::to_string(time.tm_hour); str += ":";
	str += (time.tm_min < 10 ? "0" : ""); str += std::to_string(time.tm_min); str += ":";
	str += (time.tm_sec < 10 ? "0" : ""); str += std::to_string(time.tm_sec); str += "]";

	std::string sourceInfo = from.file;
	sourceInfo += " (" + std::to_string(from.line);
	sourceInfo += ")";
	
	m_logsInfo.emplace_back(str, sourceInfo);

	switch (logLevel)
	{
	case Core::EVerbosity::LOG_TRACE:
		m_logs.push_back({ std::string("[Trace] ") + log.data(), {0.1f, 0.1f, 1.f, 1.f } });
		break;
	case Core::EVerbosity::LOG_DEBUG:
		m_logs.push_back({ std::string("[Debug] ") + log.data(), {1.f, 1.f, 1.f, 1.f } });
		break;
	case Core::EVerbosity::LOG_INFO:
		m_logs.push_back({ std::string("[Info] ") + log.data(), {0.5f, 0.5f, 0.5f, 1.f } });
		break;
	case Core::EVerbosity::LOG_WARNING:
		m_logs.push_back({ std::string("[Warning] ") + log.data(), {1.f, 1.f, 0.f, 1.f } });
		break;
	case Core::EVerbosity::LOG_ERROR:
		m_logs.push_back({ std::string("[Error] ") + log.data(), {1.f, 0.02f, 0.02f, 1.f } });
		break;
	case Core::EVerbosity::LOG_FATAL:
		m_logs.push_back({ std::string("[Fatal] ") + log.data(), {1.f, 0.f, 0.f, 1.f } });
		break;
	}		

	if (isCollapsed)
	{
		auto it = std::find(m_logsDisplayed.begin(), m_logsDisplayed.end(), m_logs.back());
		if (it != m_logsDisplayed.end())
		{
			int index = (int)std::distance(m_logsDisplayed.begin(), it);
			m_logsDisplayed[index] = m_logs.back();
			m_logsStacked[index] += 1;
			m_logsInfoDisplayed[index] = m_logsInfo.back();

			return;
		}
	}
	
	m_logsStacked.push_back(1);
	m_logsDisplayed.push_back(m_logs.back());
	m_logsInfoDisplayed.push_back(m_logsInfo.back());
}

void NIX::Editor::LogWindow::UpdateLogWindow()
{
	if (!UI::GetUI()->NewWindowWithMenuBar("Log", true)) 
	{
		UI::GetUI()->EndWindow();
		return;
	}

	if (UI::GetUI()->AddMenuBar())
	{
		if (UI::GetUI()->AddMenu("Clear"))
		{
			Clear();
			UI::GetUI()->CloseCurrentPopup();
			UI::GetUI()->CloseMenu();
		}
		if (UI::GetUI()->AddMenu("Collapse"))
		{
			if (isCollapsed)
			{
				m_logsDisplayed = m_logs;
				m_logsInfoDisplayed = m_logsInfo;
			}
			else
			{
				m_logsStacked.clear();
				m_logsStacked.reserve(m_logs.size());
				m_logsDisplayed.clear();
				m_logsStacked.reserve(m_logs.size());
				m_logsInfoDisplayed.clear();
				m_logsInfoDisplayed.reserve(m_logs.size());

				for (size_t i = 0; i < m_logs.size(); i++)
				{
					auto it = std::find(m_logsDisplayed.begin(), m_logsDisplayed.end(), m_logs[i]);
					if (it != m_logsDisplayed.end())
					{
						int index = (int)std::distance(m_logsDisplayed.begin(), it);
						m_logsStacked[index] += 1;
						m_logsInfoDisplayed[index] = m_logsInfo[i];
					}
					else
					{
						m_logsStacked.push_back(1);
						m_logsDisplayed.push_back(m_logs[i]);
						m_logsInfoDisplayed.push_back(m_logsInfo[i]);
					}
				}
			}

			isCollapsed = !isCollapsed;
			UI::GetUI()->CloseCurrentPopup();
			UI::GetUI()->CloseMenu();
		}

		UI::GetUI()->CloseMenuBar();
	}
	UI::GetUI()->Separator();
	for (size_t i = 0; i < m_logsDisplayed.size(); i++)
	{
		UI::GetUI()->AddText(m_logsInfoDisplayed[i].first);
		UI::GetUI()->SameLine();
		if (isCollapsed)
		{
			UI::GetUI()->AddText(std::to_string(m_logsStacked[i]));
			UI::GetUI()->SameLine();
		}
		UI::GetUI()->AddTextColored(m_logsDisplayed[i].first, m_logsDisplayed[i].second);
		UI::GetUI()->AddMarker(m_logsInfoDisplayed[i].second);
		UI::GetUI()->Separator();
	}

	UI::GetUI()->AutoScroll();

	UI::GetUI()->EndWindow();
}
