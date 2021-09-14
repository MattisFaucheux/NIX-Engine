#pragma once
#include "UINodeCollision.h"

namespace NIX::Editor
{
	class CollisionBox;

	class UINodeCollisionBox : public UINodeCollision
	{
	private:
		float m_scale[3] = { 1.0f, 1.0f, 1.0f };
	public:		
		void UpdateComponentProperties(GameObject* obj) override;

		void UpdateComponent() override;
	private:
		void UpdateComponentNode(bool& isComponentOpen) override;

		void UpdateScale(CollisionBox* collisionBoxComponent);
		void UpdateIsTrigger(CollisionBox* collisionBoxComponent);
		void UpdateQueryTestEnable(CollisionBox* collisionBoxComponent);
		void UpdateIsDebugDraw(CollisionBox* collisionBoxComponent);
	};
}
