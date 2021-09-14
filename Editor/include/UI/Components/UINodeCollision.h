#pragma once
#include "UINodeComponent.h"

namespace NIX::Editor
{
	class CollisionShapeComponent;

	class UINodeCollision : public UINodeComponent
	{
	protected:
		bool m_isTrigger = false;
		bool m_queryTestEnable = true;
		bool m_isDebugDraw = false;
	public:
		void UpdateComponentProperties(GameObject* obj) override = 0; 
		
		void UpdateComponent() override = 0;
	protected:
		void UpdateComponentNode(bool& isComponentOpen) override = 0;
	};
}
