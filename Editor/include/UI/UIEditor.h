#pragma once
#include "AssetsWindow.h"
#include "ContentsWindow.h"
#include "InspectorWindow.h"
#include "LogWindow.h"
#include "SceneWindow.h"
#include "Toolbar.h"

namespace NIX::Editor
{
	class UIEditor
	{
	private:
		inline static UIEditor* m_singleton = nullptr;
		
		AssetsWindow m_assetsWindow;
		ContentsWindow m_contentsWindow;
		InspectorWindow m_inspectorWindow;
		LogWindow m_logWindow;
		SceneWindow m_sceneWindow;
		Toolbar m_toolbar;
	public:
		UIEditor();
		
		static UIEditor* GetSingleton();

		static AssetsWindow& GetAssetsWindow();
		static ContentsWindow& GetContentsWindow();
		static InspectorWindow& GetInspectorWindow();
		static LogWindow& GetLogWindow();
		static SceneWindow& GetSceneWindow();
		static Toolbar& GetToolbar();
		
	private:
	};
}
