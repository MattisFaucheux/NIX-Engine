#pragma once
#include <vector>
#include <string>

#include <filesystem>

namespace NIX::Editor
{
	class Toolbar
	{
		bool m_isPlay = false;
		bool m_isPause = false;
		bool m_nextFrameMode = false;

		/// Build Window

		bool m_isBuildWindowOpen = false;

		std::vector<std::string> m_modes = { "Debug", "Release" };
		std::string m_mode = "Release";

		int m_idDrag = 0;

		std::string m_errMessageIntro;
		std::string m_errMessage;
		bool m_isErrorBuildWindowOpen = false;

		/// Mat Designer

		std::string m_mat = "";
		std::string m_materialDrag;
		std::string m_imageDrag;

		std::string m_albedo = "";
		std::string m_normal = "";

		std::vector<std::string> m_rightClickImage = { "Reset" };

		bool m_isMatWindowOpen = false;

		///

		const std::string OpenFileName(std::string_view folder, std::string_view title, std::string_view filter);
		const std::string OpenFolderName();

		void UpdateBuildWindow();
		void UpdateMatWindow();

	public:
		void SetMaterialDrag(std::filesystem::path path);
		void SetImageDrag(std::filesystem::path path);
		void OpenMatDesigner(std::string_view mat);
		void SetButtonMode(bool isPlay, bool isPause = false);
		void UpdateToolbar();
	};

}
