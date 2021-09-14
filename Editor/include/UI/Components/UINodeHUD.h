#pragma once
#include "UINodeComponent.h"

namespace NIX::Editor
{
	class Text;
	class HUDComponent;

	class UINodeHUD : public UINodeComponent
	{
	protected:
		int m_ZOrder = 0;
		float m_anchors[2] = { 0.0f, 0.0f };
		float m_pos[2] = { 0.0f, 0.0f };
	public:
		void UpdateComponentProperties(GameObject* obj) override = 0;
		void UpdateComponent() override = 0;
		
	protected:

		void UpdateComponentNode(bool& isComponentOpen) override = 0;
	};
}
