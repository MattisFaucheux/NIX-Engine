#pragma once

#include "Components/CollisionShapeComponent.h"
#include "GameObject.h"

namespace NIX
{
	namespace Physic
	{
		class CollisionSphere;
	}

	namespace Editor
	{
		class CollisionSphere : public CollisionShapeComponent
		{
		public:
			CollisionSphere(GameObject* gameObject);
			CollisionSphere(GameObject* gameObject, float radius);

			CollisionSphere(GameObject* gameObject, const CollisionSphere& other);

			void Serialize(Core::SaveElement& parent) override;
			void Deserialize(const Core::SaveElement& data) override;

			[[nodiscard]] Physic::CollisionSphere* GetCollisionSphere() const;

			static void LuaInit();

			void SetRadius(float radius);
			[[nodiscard]] float GetRadius() const;
		};
	}
}
