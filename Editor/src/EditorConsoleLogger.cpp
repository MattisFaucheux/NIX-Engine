#include "pch.h"

#include "EditorConsoleLogger.h"
#include "UI/LogWindow.h"
#include "UI/UIEditor.h"

namespace NIX::Editor
{
	void EditorConsoleLogger::PrintLog(const Core::EVerbosity verbosity, const Core::SourceInfo& sourceInfo,
		const char* message)
	{
		UIEditor::GetLogWindow().Send(verbosity, sourceInfo, message);
	}
}
