#include "pch.h"
#include "UI/AssetsWindow.h"

#include "UI/Components/UINodeModel.h"
#include "UI/Components/UINodeScript.h"
#include "UI/Toolbar.h"

#include "EditorApp.h"
#include "SceneManager.h"
#include "UI/UI.h"
#include "UI/Components/UINodeSoundEmitter.h"
#include "UI/Components/UINodeText.h"
#include "UI/Components/UINodeImage.h"
#include "UI/UIEditor.h"

void NIX::Editor::AssetsWindow::UpdateRenameWindow()
{
	bool isOpen = true;
	UI::GetUI()->SetNextWindowPos(UI::GetUI()->GetIOWidth() * .5f - 200.f, UI::GetUI()->GetIOHeight() * .5f - 100.f);
	UI::GetUI()->SetNextWindowSize(400.f, 100.f);
	UI::GetUI()->NewWindow("Rename " + m_rename.filename().string(), false, &isOpen, false);

	UI::GetUI()->AddInputText("Change name", "Type here", m_renameName);

	if (UI::GetUI()->AddButton("Save", 100.f, 20.f))
	{
		if (m_renameName.empty())
		{
			m_renameErr = "Couldn't rename to an empty name.";
		}
		else if (m_renameName == m_rename.stem().string())
		{
			m_renameErr = "This is the same name.";
		}
		else
		{
			std::string oldPath = m_rename.string();
			std::string newPath = m_rename.parent_path().string() + "\\" + m_renameName + m_rename.extension().string();

			if (rename(oldPath.c_str(), newPath.c_str()) != 0)
			{
				m_renameErr = "Error with '" + m_renameName + "' as new name.";
			}
			else
			{
				if(SceneManager::GetScenePath() == oldPath)
					SceneManager::SetScenePath(newPath);

				m_renameName = "";
				m_renameErr = "";
				m_isRenameWindowOpen = false;
			}
		}
	}
	UI::GetUI()->AddText(m_renameErr);

	UI::GetUI()->EndWindow();

	if (!isOpen)
		m_isRenameWindowOpen = false;
}

void NIX::Editor::AssetsWindow::AssetsTreeClick(std::string& path)
{
	if (!UI::GetUI()->IsTreeLabelClicked())
		return;

	m_openPath = path;
}

std::string NIX::Editor::AssetsWindow::GetIconAssets(const std::filesystem::directory_entry& path)
{
	if (path.is_directory())
		return std::string("Resources\\Images\\Icons\\IconFolder.png");

	std::string str = path.path().extension().string();

	if (str == ".png" || str == ".jpg" || str == ".tga" || str == ".mat")
		return path.path().string();
	if (str == ".obj")
		return std::string("Resources\\Images\\Icons\\IconOBJ.png");
	if (str == ".fbx")
		return std::string("Resources\\Images\\Icons\\IconFBX.png");
	if (str == ".ogg")
		return std::string("Resources\\Images\\Icons\\IconOGG.png");
	if (str == ".scene")
		return std::string("Resources\\Images\\Icons\\IconLVL.png");
	if (str == ".lua")
		return std::string("Resources\\Images\\Icons\\IconLUA.png");
	if (str == ".ttf")
		return std::string("Resources\\Images\\Icons\\IconTTF.png");

	return std::string("Resources\\Images\\Icons\\IconError.png");
}

void NIX::Editor::AssetsWindow::UpdateAssetsTree(const std::string& dir, bool& isOpenFolderComplete, std::string& openFolderPath)
{
	for (const auto& entry : std::filesystem::directory_iterator(dir))
	{
		std::string actualPath = entry.path().string();

		std::string name;

		for (size_t i = dir.length() + 1; i < actualPath.length(); i++)
			name += actualPath[i];

		if (!entry.is_directory())
			continue;

		bool isLeaf = true;

		for (const auto& element : std::filesystem::directory_iterator(actualPath))
		{
			if (element.is_directory())
			{
				isLeaf = false;
				break;
			}
		}

		bool isSelected = false;

		if (!isOpenFolderComplete)
		{
			if (openFolderPath.find_first_of('\\') != std::string::npos)
			{
				if (name == openFolderPath.substr(0, openFolderPath.find_first_of('\\')))
				{
					UI::GetUI()->SetNextItemOpen();
					openFolderPath = openFolderPath.substr(openFolderPath.find_first_of('\\') + 1);
				}
			}
			else if (name == openFolderPath)
			{
				UI::GetUI()->SetNextItemOpen();
				isOpenFolderComplete = true;
				openFolderPath.clear();
				isSelected = true;
			}
		}

		if (isLeaf)
		{
			if (UI::GetUI()->AddLeaf(name, isSelected))
			{
				AssetsTreeClick(actualPath);
				UI::GetUI()->CloseNode();
			}
		}
		else if (UI::GetUI()->AddNode(name, isSelected, true))
		{
			AssetsTreeClick(actualPath);
			UpdateAssetsTree(actualPath, isOpenFolderComplete, openFolderPath);
			UI::GetUI()->CloseNode();
		}
		else
		{
			AssetsTreeClick(actualPath);
		}
	}
}

void NIX::Editor::AssetsWindow::UpdateAssetsContents()
{
	UI::GetUI()->NewWindowWithMenuBar("AssetsIcons");
	UpdateAssetsMenuBar();
	UpdateAssetsIcons();
	UpdateRightClick();
	UI::GetUI()->EndWindow();
}

void NIX::Editor::AssetsWindow::UpdateAssetsMenuBar()
{
	if (UI::GetUI()->AddMenuBar())
	{
		std::string path;

		int nb = 0;

		while (path.length() < m_openPath.length())
		{
			size_t found = m_openPath.find_first_of('\\', path.length() + 1);
			if (found == std::string::npos)
				found = m_openPath.length();

			path = m_openPath.substr(0, found);

			found = path.find_last_of('\\');
			std::string name = path.substr(found + 1, path.length() - found);

			if (UI::GetUI()->AddMenu(name + "##" + std::to_string(nb)))
			{
				UI::GetUI()->CloseCurrentPopup();
				UI::GetUI()->CloseMenu();
				m_openPath = path;
				break;
			}

			nb++;
		}

		UI::GetUI()->CloseMenuBar();
	}
}

void NIX::Editor::AssetsWindow::UpdateAssetsIcons()
{
	int maxPerLine = 1, nbr = 1;
	const float winSize = UI::GetUI()->GetWindowWidth();
	float i = (m_sizeIcon + m_paddingIcon) * 2.f;

	while (i < winSize)
	{
		i += m_sizeIcon + m_paddingIcon;
		maxPerLine++;
	}

	std::filesystem::path temp= (std::filesystem::current_path());

	for (const auto& entry : std::filesystem::directory_iterator(m_openPath))
	{
		std::string name = entry.path().filename().string();
		std::string imagePath = GetIconAssets(entry);

		std::pair<float, float> startPos = UI::GetUI()->GetCursorPos();
		UI::GetUI()->AddImage( MainResourceManager::GetUiTextureId(imagePath), m_sizeIcon, m_sizeIcon);

		if (entry.path().extension() == ".mat")
		{
			UI::GetUI()->SetCursorPos(startPos.first, startPos.second);
			UI::GetUI()->AddImage(MainResourceManager::GetUiTextureId("Resources\\Images\\Icons\\IconMat.png"), m_sizeIcon, m_sizeIcon);
		}

		UI::GetUI()->SetCursorPos(startPos.first, startPos.second);

		if (!entry.is_directory())
		{
			name = entry.path().stem().string();

			if (UI::GetUI()->AddSelectableDoubleClick( "##" + name, m_sizeIcon, m_sizeIcon + m_paddingIcon * .5f))
			{
				if (entry.path().extension() == ".scene")
				{
					SceneManager::LoadScene(entry.path().string().c_str());
				}
				else if (entry.path().extension() == ".mat")
				{
					UIEditor::GetToolbar().OpenMatDesigner(entry.path().string());
				}
				else
				{
					std::string str = "explorer /n, /select," + entry.path().string();
					system(str.c_str());
				}
			}
			if (entry.path().extension() == ".mat" && UI::GetUI()->DragDropSource("DragDropMaterials", name))
			{
				UIEditor::GetInspectorWindow().GetUINodeModel().SetMaterialDrag(entry.path());
				UIEditor::GetToolbar().SetMaterialDrag(entry.path());
			}
			else if ((entry.path().extension() == ".obj" || entry.path().extension() == ".fbx") && UI::GetUI()->DragDropSource("DragDropMeshes", name))
			{
				UIEditor::GetInspectorWindow().GetUINodeModel().SetMeshDrag(entry.path());
			}
			else if ((entry.path().extension() == ".png" || entry.path().extension() == ".jpg" || entry.path().extension() == ".tga") && UI::GetUI()->DragDropSource("DragDropImages", name))
			{
				UIEditor::GetInspectorWindow().GetUINodeImage().SetImageDrag(entry.path());
				UIEditor::GetToolbar().SetImageDrag(entry.path());
			}
			else if (entry.path().extension() == ".lua" && UI::GetUI()->DragDropSource("DragDropScripts", name))
			{
				UIEditor::GetInspectorWindow().GetUINodeScript().SetScriptDrag(entry.path());
			}
			else if (entry.path().extension() == ".ttf" && UI::GetUI()->DragDropSource("DragDropFonts", name))
			{
				UIEditor::GetInspectorWindow().GetUINodeText().SetFontDrag(entry.path());
			}
			else if (entry.path().extension() == ".ogg" && UI::GetUI()->DragDropSource("DragDropSounds", name))
			{
				UIEditor::GetInspectorWindow().GetUINodeSoundEmitter().SetSoundDrag(entry.path());
			}
			else if (entry.path().extension() == ".wav" && UI::GetUI()->DragDropSource("DragDropSounds", name))
			{
				UIEditor::GetInspectorWindow().GetUINodeSoundEmitter().SetSoundDrag(entry.path());
			}
			else if (entry.path().extension() == ".mp3" && UI::GetUI()->DragDropSource("DragDropSounds", name))
			{
				UIEditor::GetInspectorWindow().GetUINodeSoundEmitter().SetSoundDrag(entry.path());
			}
		}
		else if (UI::GetUI()->AddSelectableDoubleClick( "##" + name, m_sizeIcon, m_sizeIcon + m_paddingIcon * .5f))
		{
			m_openPath = entry.path().string();
		}

		if (UI::GetUI()->IsClicked(1))
		{
			UI::GetUI()->OpenPopup("##RightClickAssetsIcon");
			m_isRightClickIcon = true;
			m_rightClicked = entry.path();
		}

		if (name.length() > m_maxCharIcon + 3)
			name = name.substr(0, m_maxCharIcon) + "...";

		const float addCursorX = (m_sizeIcon - UI::GetUI()->CalcTextSize(name).first) * .5f;
		UI::GetUI()->SetCursorPos(startPos.first + addCursorX, startPos.second + m_sizeIcon);
		UI::GetUI()->AddText(name);


		if (nbr % maxPerLine)
			UI::GetUI()->SetCursorPos(startPos.first + m_sizeIcon + m_paddingIcon, startPos.second);
		else
			UI::GetUI()->SetCursorPosY(startPos.second + m_sizeIcon + m_paddingIcon);

		nbr++;
	}
}

void NIX::Editor::AssetsWindow::UpdateRightClick()
{
	int res = -1;

	if (m_isRightClickIcon)
		res = UI::GetUI()->AddPopup("##RightClickAssetsIcon", m_rightClickOnIcon);

	if (res != -1)
		m_isRightClickIcon = false;

	switch (res)
	{
	case 0:
	{
		std::string str = "explorer /n, /select," + m_rightClicked.string();
		system(str.c_str());
		break;
	}
	case 1:
	{
		m_isRenameWindowOpen = true;
		m_renameErr = "";
		m_rename = m_rightClicked;
		break;
	}
	case 3:
	{
		m_copy = m_rightClicked;
		break;
	}
	case 5:
	{
		std::filesystem::remove_all(m_rightClicked);
		break;
	}
	default:
		break;
	}

	res = -1;

	if (!m_isRightClickIcon)
		res = UI::GetUI()->AddPopupWindow("##RightClickAssetsWindow", m_rightClick);

	switch (res)
	{
	case 0:
	{
		if (std::filesystem::exists(m_copy) && std::filesystem::absolute(m_openPath).string().find(std::filesystem::absolute(m_copy).string()) == std::string::npos)
		{
			if (std::filesystem::is_directory(m_copy))
			{
				std::string newFolder = m_openPath + "\\" + m_copy.filename().string();
				int nb = 0;

				while (std::filesystem::exists(newFolder))
				{
					nb++;
					newFolder = m_openPath + "\\" + m_copy.stem().string() + std::to_string(nb) + m_copy.extension().string();
				}

				std::filesystem::create_directory(newFolder);
				std::filesystem::copy(m_copy, newFolder + "\\", std::filesystem::copy_options::recursive);
			}
			else
			{
				if (m_copy.parent_path().string() != m_openPath)
				{
					std::filesystem::copy(m_copy, m_openPath, std::filesystem::copy_options::recursive);
				}
				else
				{
					std::string newName = m_openPath;
					int nb = 0;

					while (std::filesystem::exists(newName))
					{
						nb++;
						newName = m_copy.parent_path().string() + "\\" + m_copy.stem().string() + std::to_string(nb) + m_copy.extension().string();
					}

					std::filesystem::copy(m_copy, newName, std::filesystem::copy_options::recursive);
				}
			}
		}
		break;
	}
	case 2:
	{
		std::string name = m_openPath + "\\New Folder";
		int nb = 0;

		while (std::filesystem::exists(name))
		{
			nb++;
			name = m_openPath + "\\New Folder " + std::to_string(nb);
		}

		std::filesystem::create_directory(name);
		break;
	}
	case 4:
	{
		std::string from = "Resources\\Scenes\\defaultScene.scene";
		std::string to = m_openPath + "\\defaultScene.scene";
		int nb = 0;

		while (std::filesystem::exists(to))
		{
			nb++;
			to = m_openPath + "\\defaultScene" + std::to_string(nb) + ".scene";
		}
		std::filesystem::copy(from, to);
		break;
	}
	case 5:
	{
		std::string from = "Resources\\Materials\\defaultMaterial.mat";
		std::string to = m_openPath + "\\defaultMaterial.mat";
		int nb = 0;

		while (std::filesystem::exists(to))
		{
			nb++;
			to = m_openPath + "\\defaultMaterial" + std::to_string(nb) + ".mat";
		}

		std::filesystem::copy(from, to);
		break;
	}
	case 6:
	{
		std::string from = "Resources\\Scripts\\defaultScript.lua";
		std::string to = m_openPath + "\\defaultScript.lua";
		int nb = 0;

		while (std::filesystem::exists(to))
		{
			nb++;
			to = m_openPath + "\\defaultScript" + std::to_string(nb) + ".lua";
		}

		std::filesystem::copy(from, to);
		break;
	}
	default:
		break;
	}
}

void NIX::Editor::AssetsWindow::UpdateAssetsWindow()
{
	if (!UI::GetUI()->NewWindow("AssetsTree", true))
	{
		UI::GetUI()->EndWindow();
		return;
	}

	std::string name = "Assets";
	bool isSelected = false;

	if (m_openPath.length() > name.length())
		UI::GetUI()->SetNextItemOpen();
	else
		isSelected = true;

	if (UI::GetUI()->AddNode(name, isSelected, true))
	{
		AssetsTreeClick(name);		
		bool isOpenFolderComplete = false;
		std::string openFolderPath = m_openPath.substr(m_openPath.find_first_of('\\') + 1);
		UpdateAssetsTree(name, isOpenFolderComplete, openFolderPath);
		UI::GetUI()->CloseNode();
	}
	UI::GetUI()->EndWindow();
	UpdateAssetsContents();
	if (m_isRenameWindowOpen) UpdateRenameWindow();
}