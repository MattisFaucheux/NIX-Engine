#include "UI/UIEditor.h"

#include "Log/Logger.h"

namespace NIX::Editor
{
	UIEditor::UIEditor()
	{
		if (m_singleton != nullptr)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_TRACE, "An other instance of UIEditor tried to be created");
			return;
		}

		m_singleton = this;
	}

	UIEditor* UIEditor::GetSingleton()
	{
		if (m_singleton == nullptr)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_TRACE, "Instance of UIEditor is null, need to be construct one time");
		}
		
		return m_singleton;
	}

	AssetsWindow& UIEditor::GetAssetsWindow()
	{		
		return m_singleton->m_assetsWindow;
	}

	ContentsWindow& UIEditor::GetContentsWindow()
	{
		return m_singleton->m_contentsWindow;
	}

	InspectorWindow& UIEditor::GetInspectorWindow()
	{
		return m_singleton->m_inspectorWindow;
	}
	
	LogWindow& UIEditor::GetLogWindow()
	{
		return m_singleton->m_logWindow;
	}
	
	SceneWindow& UIEditor::GetSceneWindow()
	{
		return m_singleton->m_sceneWindow;
	}

	Toolbar& UIEditor::GetToolbar()
	{
		return m_singleton->m_toolbar;
	}
	
}
