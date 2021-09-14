#pragma once
#include <string>
#include <filesystem>

namespace NIX::Editor
{
	class AssetsWindow
	{
		std::string m_openPath = "Assets";
		std::vector<std::string> m_rightClick = { "Paste", "##Separator", "Create Folder", "##Separator", "Create Scene", "Create Material", "Create Script" };
		std::vector<std::string> m_rightClickOnIcon = { "Open", "Rename", "##Separator", "Copy", "##Separator", "Delete" };

		bool m_isRightClickIcon = false;
		bool m_isRenameWindowOpen = false;
		std::filesystem::path m_rightClicked = "";
		std::filesystem::path m_copy = "";
		std::filesystem::path m_rename = "";
		std::string m_renameName = "";
		std::string m_renameErr = "";

		float m_sizeIcon = 100.f;
		float m_paddingIcon = 25.f;
		size_t m_maxCharIcon = 10;

		void UpdateRenameWindow();

		void AssetsTreeClick(std::string& path);
		std::string GetIconAssets(const std::filesystem::directory_entry&);

		void UpdateAssetsTree(const std::string& dir, bool& isOpenFolderComplete, std::string& openFolderPath);
		void UpdateAssetsContents();
		void UpdateAssetsMenuBar();
		void UpdateAssetsIcons();
		void UpdateRightClick();

	public:
		void UpdateAssetsWindow();
	};

}
