#include "pch.h"
#include "UI/Toolbar.h"

#include "UI/UI.h"
#include "UI/HUDDesigner.h"

#include <Windows.h>
#include <iostream>
#include <filesystem>
#include <ShlObj_core.h>

#include "EditorApp.h"
#include "ResourceManager/MainResourceManager.h"
#include "ResourceManager/MaterialManager.h"
#include "ResourceManager/TextureViewManager.h"
#include "SceneManager.h"

#include "Material/Material.h"

#include "Serialization/AssetsParsing/ParseMaterial.h"


const std::string NIX::Editor::Toolbar::OpenFileName(std::string_view folder, std::string_view title, std::string_view filter)
{
	std::string file = std::filesystem::absolute(folder).string();

	OPENFILENAMEA ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAMEA));
	char szFile[MAX_PATH] = { 0 };

	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;
	ofn.hwndOwner = NULL;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = title.data();
	ofn.nFilterIndex = 0;
	ofn.lpstrDefExt = NULL;
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;
	ofn.lpstrFilter = filter.data();
	ofn.lpstrInitialDir = file.c_str();

	std::string result = "";

	if (GetOpenFileNameA(&ofn))
	{
		result = ofn.lpstrFile;
		result.erase(result.begin(), result.begin() + file.size() - 6);
	}

	return result;
}

const std::string NIX::Editor::Toolbar::OpenFolderName()
{
	BROWSEINFOA bi;
	ZeroMemory(&bi, sizeof(BROWSEINFOA));
	char szFolder[MAX_PATH] = { 0 };

	bi.pidlRoot = NULL;
	bi.pszDisplayName = szFolder;
	bi.lpszTitle = "Please, select a folder";
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = -1;

	std::string result = "";

	LPITEMIDLIST pidl = SHBrowseForFolderA(&bi);
	if (pidl != NULL)
	{
		SHGetPathFromIDListA(pidl, szFolder);
		result = szFolder;
	}

	return result;
}

void NIX::Editor::Toolbar::UpdateBuildWindow()
{
	bool isOpen = true;
	UI::GetUI()->SetNextWindowPos(UI::GetUI()->GetIOWidth() * .5f - 200.f, UI::GetUI()->GetIOHeight() * .5f - 350.f);
	UI::GetUI()->SetNextWindowSize(400.f, 500.f);
	UI::GetUI()->NewWindow("Build Settings", false, &isOpen, false);

	std::string buildName = EditorApp::GetShippingSettings().GetBuildName().data();
	if (UI::GetUI()->AddInputText("Build Name", "", buildName))
	{
		EditorApp::GetShippingSettings().SetBuildName(buildName);
	}

	UI::GetUI()->SetCursorPosY(UI::GetUI()->GetCursorPosY() + 20.f);
	std::string visualFolder = EditorApp::GetShippingSettings().GetVisualStudioFolder().data();
	UI::GetUI()->AddInputText("Visual Folder", "", visualFolder, true);
	if (UI::GetUI()->AddButton("Select Visual Folder", 150.f, 20.f))
	{
		const std::string result = OpenFolderName();

		if (!result.empty())
		{
			EditorApp::GetShippingSettings().SetVisualStudioFolder(result);
		}
	}

	std::string buildFolder = EditorApp::GetShippingSettings().GetBuildFolder().data();
	UI::GetUI()->AddInputText("Build Folder", "", buildFolder, true);
	if (UI::GetUI()->AddButton("Select Build Folder", 150.f, 20.f))
	{
		const std::string result = OpenFolderName();

		if (!result.empty())
		{
			EditorApp::GetShippingSettings().SetBuildFolder(result);
		}
	}

	UI::GetUI()->SetCursorPosY(UI::GetUI()->GetCursorPosY() + 20.f);

	m_mode = "Release";
	if (EditorApp::GetShippingSettings().GetShippingMode() == EShippingMode::DEBUG)
		m_mode = "Debug";

	if (UI::GetUI()->AddCombo("Build Mode", m_modes, m_mode))
	{
		EditorApp::GetShippingSettings().SetShippingMode(m_mode == "Release" ? EShippingMode::RELEASE : EShippingMode::DEBUG);
	}

	UI::GetUI()->SetCursorPosY(UI::GetUI()->GetCursorPosY() + 20.f);
	UI::GetUI()->AddText("Scenes In Build");
	UI::GetUI()->NewChild("##ScenesInBuild", 0.f, 200.f, true);

	int id = 0;
	for (auto scene : EditorApp::GetShippingSettings().GetScenesInBuild())
	{
		UI::GetUI()->AddSelectable(std::to_string(id) + " | " + scene, UI::GetUI()->GetContentRegionAvailX() - 25.f, 20.f);
		if (UI::GetUI()->DragDropSource("DragDropSceneBuild", scene))
		{
			m_idDrag = id;
		}
		if (UI::GetUI()->DragDropTarget("DragDropSceneBuild"))
		{
			EditorApp::GetShippingSettings().SwapSceneToBuild(m_idDrag, id);
		}

		UI::GetUI()->SameLine();
		UI::GetUI()->SetCursorPos(UI::GetUI()->GetCursorPosX() + UI::GetUI()->GetContentRegionAvailX() - 20.f, UI::GetUI()->GetCursorPosY());

		if (UI::GetUI()->AddButton("X##DeleteTag" + std::to_string(id), 20.f, 20.f))
		{
			EditorApp::GetShippingSettings().RemoveSceneInBuild(id);
		}

		id++;
	}

	UI::GetUI()->EndChild();

	UI::GetUI()->SetCursorPosY(UI::GetUI()->GetCursorPosY() + 10.f);
	if (UI::GetUI()->AddButton("Add Scene", 100.f, 30.f))
	{
		const std::string result = OpenFileName("Assets", "Select the scene to add\0", "Scene (.scene)\0*.scene");

		if (!result.empty())
		{
			for (auto scene : EditorApp::GetShippingSettings().GetScenesInBuild())
			{
				if (scene == result)
				{
					m_isErrorBuildWindowOpen = true;
					m_errMessageIntro = "Couldn't add " + result + ".";
					m_errMessage = "This scene already exists in the build settings.";
					break;
				}
			}

			if (!m_isErrorBuildWindowOpen)
			{
				int idAdd = (int)EditorApp::GetShippingSettings().GetScenesInBuild().size();
				EditorApp::GetShippingSettings().AddSceneToBuild(idAdd, result);
			}
		}
	}

	UI::GetUI()->SameLine();
	UI::GetUI()->SetCursorPosX(UI::GetUI()->GetContentRegionAvailX() + UI::GetUI()->GetCursorPosX() - 100.f);
	if (UI::GetUI()->AddButton("Build", 100.f, 30.f))
	{
		EditorApp::GetShippingSettings().CreateAndRunBuildingScript();
	}

	UI::GetUI()->EndWindow();

	if (!isOpen)
		m_isBuildWindowOpen = false;
}

void NIX::Editor::Toolbar::UpdateMatWindow()
{
	bool isOpen = true;
	UI::GetUI()->SetNextWindowPos(UI::GetUI()->GetIOWidth() * .5f - 200.f, UI::GetUI()->GetIOHeight() * .5f - 200.f);
	UI::GetUI()->SetNextWindowSize(400.f, 200.f);
	UI::GetUI()->NewWindow("Material Designer", false, &isOpen, false);

	if (m_mat.empty())
	{
		UI::GetUI()->SetCursorPos(200.f - UI::GetUI()->CalcTextSize("Please, drop the material to edit:").first * .5f, 40.f);
		UI::GetUI()->AddText("Please, drop the material to edit:");
		UI::GetUI()->SetCursorPos(150.f, 70.f);
		UI::GetUI()->AddImage(MainResourceManager::GetUiTextureId("Default Texture"), 100.f, 100.f);
		if (UI::GetUI()->DragDropTarget("DragDropMaterials"))
		{
			m_mat = m_materialDrag;

			Rendering::Material* mat = MainResourceManager::GetMaterialManager()->Find(m_mat.c_str());

			m_albedo = mat->albedoMap->GetPath();

			if (mat->isNormalMap)
				m_normal = mat->normalMap->GetPath();
			else
				m_normal = "";
		}
		std::pair txt = UI::GetUI()->CalcTextSize("Drop here...");
		UI::GetUI()->SetCursorPos(200.f - txt.first * .5f, 120.f - txt.second * .5f);
		UI::GetUI()->AddTextColored("Drop here...", { 0.05f, 0.05f, 0.05f, 1.f});
	}
	else
	{
		UI::GetUI()->SetCursorPosY(30.f);
		UI::GetUI()->AddText("Material: " + std::filesystem::path(m_mat).stem().string());

		const float posY = 70.f;

		UI::GetUI()->SetCursorPosY(posY);
		UI::GetUI()->AddText("Albedo:");
		UI::GetUI()->SetCursorPos(80.f, posY - 5.f);

		if (!m_albedo.empty())
			UI::GetUI()->AddImage(MainResourceManager::GetUiTextureId(m_albedo), 20.f, 20.f);
		else
			UI::GetUI()->AddImage(MainResourceManager::GetUiTextureId("Default Texture"), 20.f, 20.f);

		if (UI::GetUI()->DragDropTarget("DragDropImages"))
		{
			m_albedo = m_imageDrag;

			Rendering::Material* mat = MainResourceManager::GetMaterialManager()->Find(m_mat.c_str());

			if (!m_albedo.empty())
				mat->UpdateAlbedoMap(MainResourceManager::GetTextureViewManager()->Find(m_albedo.c_str()));

			Core::ParseMaterial::CreateMaterial(m_mat.c_str(), m_albedo.c_str(), m_normal.c_str());

			
			MainResourceManager::GetTextureViewManager()->UpdateTexturePtr(m_mat.c_str(), mat->CreateMaterialPreview());

		}
		if (m_albedo != "Default Texture" && UI::GetUI()->IsClicked(1))
		{
			UI::GetUI()->OpenPopup("##RightClickedAlbedoPath");
		}
		if (UI::GetUI()->AddPopup("##RightClickedAlbedoPath", m_rightClickImage) == 0)
		{
			m_albedo = "Default Texture";

			Rendering::Material* mat = MainResourceManager::GetMaterialManager()->Find(m_mat.c_str());
			mat->albedoMap = MainResourceManager::GetTextureViewManager()->Find(m_albedo.c_str());
		}

		UI::GetUI()->SetCursorPos(115.f, posY);

		if (!m_albedo.empty())
			UI::GetUI()->AddText(std::filesystem::path(m_albedo).stem().string());
		else
			UI::GetUI()->AddText("(Not assigned)");
		
		UI::GetUI()->SetCursorPosY(posY + 25.f);
		UI::GetUI()->AddText("Normal:");
		UI::GetUI()->SetCursorPos(80.f, posY + 20.f);

		if(!m_normal.empty())
			UI::GetUI()->AddImage(MainResourceManager::GetUiTextureId(m_normal), 20.f, 20.f);
		else
			UI::GetUI()->AddImage(MainResourceManager::GetUiTextureId("Default Texture"), 20.f, 20.f);

		if (UI::GetUI()->DragDropTarget("DragDropImages"))
		{
			m_normal = m_imageDrag;

			Rendering::Material* mat = MainResourceManager::GetMaterialManager()->Find(m_mat.c_str());

			if (!m_normal.empty())
			{
				mat->UpdateNormalMap(MainResourceManager::GetTextureViewManager()->Find(m_normal.c_str()));
				Core::ParseMaterial::CreateMaterial(m_mat.c_str(), m_albedo.c_str(), m_normal.c_str());
			}
			else
			{
				mat->UpdateNormalMap(nullptr);
				Core::ParseMaterial::CreateMaterial(m_mat.c_str(), m_albedo.c_str(), nullptr);
			}

			
		}
		if (m_normal != "Default Texture" && UI::GetUI()->IsClicked(1))
		{
			UI::GetUI()->OpenPopup("##RightClickedNormalPath");
		}
		if (UI::GetUI()->AddPopup("##RightClickedNormalPath", m_rightClickImage) == 0)
		{
			m_normal = "Default Texture";

			Rendering::Material* mat = MainResourceManager::GetMaterialManager()->Find(m_mat.c_str());

			mat->normalMap = MainResourceManager::GetTextureViewManager()->Find(m_normal.c_str());
			mat->isNormalMap = true;
		}

		UI::GetUI()->SetCursorPos(115.f, posY + 25.f);

		if (!m_normal.empty())
			UI::GetUI()->AddText(std::filesystem::path(m_normal).stem().string());
		else
			UI::GetUI()->AddText("(Not assigned)");

		UI::GetUI()->SetCursorPos(10.f, UI::GetUI()->GetWindowHeight() - 30.f);
		if (UI::GetUI()->AddButton("<- Edit another Material", 180.f, 20.f))
		{
			m_mat = "";
			m_albedo = "";
			m_normal = "";
		}
	}

	UI::GetUI()->EndWindow();

	if (!isOpen)
		m_isMatWindowOpen = false;
}

void NIX::Editor::Toolbar::SetMaterialDrag(std::filesystem::path path)
{
	m_materialDrag = path.string();
}

void NIX::Editor::Toolbar::SetImageDrag(std::filesystem::path path)
{
	m_imageDrag = path.string();
}

void NIX::Editor::Toolbar::OpenMatDesigner(std::string_view mat)
{
	m_isMatWindowOpen = true;
	m_mat = mat;

	Rendering::Material* material = MainResourceManager::GetMaterialManager()->Find(m_mat.c_str());

	m_albedo = material->albedoMap->GetPath();

	if (material->isNormalMap)
		m_normal = material->normalMap->GetPath();
	else
		m_normal = "";
}

void NIX::Editor::Toolbar::SetButtonMode(bool isPlay, bool isPause)
{
	m_isPlay = isPlay;
	m_isPause = isPause;
}

void NIX::Editor::Toolbar::UpdateToolbar()
{
	UI::GetUI()->PushFramePadding(UI::GetUI()->GetContentRegionAvailX(), 10.f);
	UI::GetUI()->AddMainMenuBar();
	UI::GetUI()->PopStyle();

	float posX = UI::GetUI()->GetContentRegionAvailX() * .5f - 75.f;

	UI::GetUI()->PushFramePadding(20.f, 10.f);

	if(UI::GetUI()->AddMenu("Save"))
	{
		EditorApp::SaveScene();

		UI::GetUI()->CloseCurrentPopup();
		UI::GetUI()->CloseMenu();
	}
	if (UI::GetUI()->AddMenu("Load"))
	{
		const std::string result = OpenFileName("Assets", "Please, select a scene to load", "Scene (.scene)\0*.scene");

		if (!result.empty())
		{
			SceneManager::LoadScene(result.c_str());
		}

		UI::GetUI()->CloseCurrentPopup();
		UI::GetUI()->CloseMenu();
	}
	if (UI::GetUI()->AddMenu("Build"))
	{
		m_isBuildWindowOpen = !m_isBuildWindowOpen;		
		UI::GetUI()->CloseCurrentPopup();
		UI::GetUI()->CloseMenu();
	}
	if (UI::GetUI()->AddMenu("Designers"))
	{
		if (UI::GetUI()->AddMenuItem("HUD Designer", HUDDesigner::IsOpen()))
		{
			HUDDesigner::IsOpen(!HUDDesigner::IsOpen());
		}
		if (UI::GetUI()->AddMenuItem("Material Designer", m_isMatWindowOpen))
		{
			m_isMatWindowOpen = !m_isMatWindowOpen;
			m_mat = "";
		}

		UI::GetUI()->CloseMenu();
	}

	UI::GetUI()->AddText(" |   " + std::filesystem::path(SceneManager::GetScenePath()).filename().string());

	UI::GetUI()->PopStyle();

	UI::GetUI()->SetCursorPos(posX, 4.f);

	if (m_isPlay)
	{
		if (m_isPause)
		{
			UI::GetUI()->AddImage(MainResourceManager::GetUiTextureId("Resources\\Images\\Icons\\PlayOnPauseOnNext.png"), 150.f, 25.f);
		}
		else
		{
			UI::GetUI()->AddImage(MainResourceManager::GetUiTextureId("Resources\\Images\\Icons\\PlayOnPauseNext.png"), 150.f, 25.f);
		}
	}
	else
	{
		UI::GetUI()->AddImage(MainResourceManager::GetUiTextureId("Resources\\Images\\Icons\\PlayPauseNext.png"), 150.f, 25.f);
	}

	if (m_nextFrameMode)
	{
		m_isPause = EditorApp::SwitchPauseMode();
		m_nextFrameMode = false;
	}

	posX += 4.f;
	UI::GetUI()->SetCursorPos(posX, -4.f);
	if (UI::GetUI()->AddSelectable("##ToolbarPlay", 42.f, 21.f))
	{
		m_isPlay = EditorApp::SwitchEngineState();
		if (!m_isPlay) m_isPause = false;
	}
	posX += 50.f;
	UI::GetUI()->SetCursorPos(posX, -4.f);
	if (UI::GetUI()->AddSelectable("##ToolbarPause", 42.f, 21.f))
	{
		m_isPause = EditorApp::SwitchPauseMode();
	}
	posX += 50.f;
	UI::GetUI()->SetCursorPos(posX, -4.f);
	if (UI::GetUI()->AddSelectable("##ToolbarNext", 42.f, 21.f))
	{
		m_nextFrameMode = EditorApp::SwitchNextFrameMode();
	}

	if(m_isBuildWindowOpen) UpdateBuildWindow();

	if(m_isMatWindowOpen) UpdateMatWindow();

	if (m_isErrorBuildWindowOpen)
	{
		bool isOpen = true;
		UI::GetUI()->SetNextWindowPos(UI::GetUI()->GetIOWidth() * .5f - 300.f, UI::GetUI()->GetIOHeight() * .5f - 100.f);
		UI::GetUI()->SetNextWindowSize(600.f, 100.f);
		UI::GetUI()->NewWindow("Failed to add new scene", false, &isOpen, false);
		UI::GetUI()->AddText(m_errMessageIntro);
		UI::GetUI()->AddText(m_errMessage);
		UI::GetUI()->EndWindow();

		if (!isOpen)
			m_isErrorBuildWindowOpen = false;
	}

	UI::GetUI()->CloseMainMenuBar();
}
