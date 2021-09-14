#pragma once
#include <string>
#include <vector>

#include "UINodeHUD.h"

namespace std {
	namespace filesystem {
		class path;
	}
}

namespace NIX::Editor
{
	class Text;
	class GameObject;

	class UINodeText : public UINodeHUD
	{
	private:		
		std::string m_text = "";
		int m_size = 16;
		float m_color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		std::string m_fontPath = "";
		std::string m_fontDrag;

		/// Right-Click

		std::vector<std::string> m_rightClick = { "Reset" };

		///
	public:		
		void UpdateComponentProperties(GameObject* obj) override;

		void UpdateComponent() override;

		void SetFontDrag(std::filesystem::path path);

	private:
		void UpdateComponentNode(bool& isComponentOpen) override;

		void UpdatePos(Text* text);
		void UpdateAnchors(Text* text);
		void UpdateZOrder(Text* text);
		void UpdateText(Text* text);
		void UpdateColor(Text* text);
		void UpdateFont(Text* text);
		void UpdateSize(Text* text);
		
	};
}
