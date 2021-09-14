#include "pch.h"
#include "UI/SceneWindow.h"

#include "UI/UI.h"
#include "UI/HUDDesigner.h"
#include "EditorApp.h"

std::pair<float, float> NIX::Editor::SceneWindow::UpdateSceneWindow(void *& textureID)
{
	UI::GetUI()->NoPadding();

#ifndef _SHIPPING
	if (!UI::GetUI()->NewWindow("Scene"), false, nullptr, true, false)
	{
		UI::GetUI()->EndWindow();
		UI::GetUI()->PopStyle();
		return std::pair<float, float>(0.f, 0.f);
	}
#endif
	
#ifdef _SHIPPING
	UI::GetUI()->NewWindow("Scene", false, nullptr, false, true);
#endif

	UI::GetUI()->SetCursorPosX(0.f);

	float top = UI::GetUI()->GetCursorPosY();

	std::pair<float, float> sceneSize = UI::GetUI()->GetContentRegionAvail();
	
	if (textureID != 0)
	{
		UI::GetUI()->AddImage(textureID, sceneSize.first, sceneSize.second);
	}

	if (EditorApp::GetActualState() != Editor::EngineState::EDITOR)
	{
		UI::GetUI()->SetCursorPos(0.f, top);
		HUDDesigner::UpdateHUDDesigner("##HUD");
	}
	else
	{
		std::pair<float, float> nextPos = UI::GetUI()->GetWindowPos();
		UI::GetUI()->SetNextWindowSize(UI::GetUI()->GetWindowWidth(), UI::GetUI()->GetWindowHeight());
		UI::GetUI()->SetNextWindowPos(nextPos.first, nextPos.second);

		bool isOpen = true;

		if (HUDDesigner::IsOpen())
		{
			UI::GetUI()->PushWindowBgColor({ 0.1f, 0.1f, 0.1f, .2f });
			UI::GetUI()->NewWindow("HUD Designer", false, &isOpen, false);
			HUDDesigner::UpdateHUDDesigner("##HUDDesigner");
			UI::GetUI()->EndWindow();
			UI::GetUI()->PopWindowBgColor();
		}

		if (!isOpen)
			HUDDesigner::IsOpen(false);
	}
	
	UI::GetUI()->EndWindow();

	UI::GetUI()->PopStyle();

	return sceneSize;
}
