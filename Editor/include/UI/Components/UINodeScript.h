#pragma once

#include <filesystem>

#include "UINodeComponent.h"

namespace NIX::Editor
{
	class Script;
	class GameObject;

	class UINodeScript : public UINodeComponent
	{
	private:		
		std::string m_scriptDrag;

		std::vector<std::string> m_rightClick = { "Remove" };
		std::string m_rightClicked = "";

	public:		
		void UpdateComponentProperties(GameObject* obj) override;

		void UpdateComponent() override;

		void SetScriptDrag(std::filesystem::path path);
		
	private:
		void UpdateComponentNode(bool& isComponentOpen) override;
		
		void UpdateScripts(Script* scriptComponent);
	};


}
