#pragma once
#include "UINodeCollision.h"

namespace NIX::Editor
{
	class CollisionMesh;

	class UINodeCollisionConvex : public UINodeCollision
	{
	public:
		void UpdateComponentProperties(GameObject* obj) override;

		void UpdateComponent() override;
	private:
		void UpdateComponentNode(bool& isComponentOpen) override;

		void UpdateIsTrigger(CollisionMesh* collisionConvexComp);
		void UpdateQueryTestEnable(CollisionMesh* collisionConvexComp);
		void UpdateIsDebugDraw(CollisionMesh* collisionConvexComp);
	};
}