#pragma once
#include "UINodeCollision.h"

namespace NIX::Editor
{
	class CollisionCapsule;

	class UINodeCollisionCapsule : public UINodeCollision
	{
	private:
		float m_radius = 1.0f;
		float m_height = 1.0f;
	public:		
		void UpdateComponentProperties(GameObject* obj) override;
		
		void UpdateComponent() override;
	private:
		void UpdateComponentNode(bool& isComponentOpen) override;

		void UpdateRadius(CollisionCapsule* collisionCapsuleComponent);
		void UpdateHeight(CollisionCapsule* collisionCapsuleComponent);
		void UpdateIsTrigger(CollisionCapsule* collisionCapsuleComponent);
		void UpdateQueryTestEnable(CollisionCapsule* collisionCapsuleComponent);
		void UpdateIsDebugDraw(CollisionCapsule* collisionCapsuleComponent);
	};
}
