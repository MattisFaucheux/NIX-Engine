#pragma once

#include "Components/CollisionShapeComponent.h"
#include "GameObject.h"

namespace NIX
{
	namespace Physic
	{
		class CollisionShape;
	}

	namespace Editor
	{
		class CollisionShapeComponent : public Component
		{
		public:

			CollisionShapeComponent(GameObject* gameObject, Physic::CollisionShape* collisionShape);
			CollisionShapeComponent(GameObject* gameObject);

			void Serialize(Core::SaveElement& parent) override;
			void Deserialize(const Core::SaveElement& data) override;

			void SetIsActive(bool value) override;

			[[nodiscard]] Physic::CollisionShape* GetCollisionShape();

			~CollisionShapeComponent() override;

			static void LuaInit();

			void SetIsQueryTestEnable(bool isEnable);
			bool GetIsQueryTestEnable() const;

			void SetIsTrigger(bool isTrigger);
			[[nodiscard]] bool GetIsTrigger() const;

			void SetIsDebugDraw(bool isDebugDraw);
			[[nodiscard]] bool GetIsDebugDraw() const;

			bool IsSameCollisionShape(Physic::CollisionShape* other);

		protected:

			Physic::CollisionShape* m_collisionShape = nullptr;
		};
	}
}
