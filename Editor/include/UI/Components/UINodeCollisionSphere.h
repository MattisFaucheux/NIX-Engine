#pragma once
#include "UINodeCollision.h"

namespace NIX::Editor
{
	class CollisionSphere;

	class UINodeCollisionSphere : public UINodeCollision
	{
	private:
		float m_radius = 1.0f;
	public: 
		void UpdateComponentProperties(GameObject* obj) override;

		void UpdateComponent() override;
	private:
		void UpdateComponentNode(bool& isComponentOpen) override;

		void UpdateRadius(CollisionSphere* collisionSphereComponent);
		void UpdateIsTrigger(CollisionSphere* collisionSphereComponent);
		void UpdateQueryTestEnable(CollisionSphere* collisionSphereComponent);
		void UpdateIsDebugDraw(CollisionSphere* collisionSphereComponent);
	};
}