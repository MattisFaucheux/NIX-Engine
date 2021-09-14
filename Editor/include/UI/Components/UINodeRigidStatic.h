#pragma once
#include "UINodeComponent.h"

namespace NIX::Editor
{
	class UINodeRigidStatic : UINodeComponent
	{		
	public:
		
		void UpdateComponentProperties(GameObject* obj) override;

		void UpdateComponent() override;
	private:

		void UpdateComponentNode(bool& isComponentOpen) override;
	};
}
