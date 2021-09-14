#pragma once

#include "Components/CollisionShapeComponent.h"
#include "GameObject.h"

namespace NIX
{
	namespace Physic
	{
		class CollisionCapsule;
	}

	namespace Editor
	{
		class CollisionCapsule : public CollisionShapeComponent
		{
		public:
			CollisionCapsule(GameObject* gameObject);
			CollisionCapsule(GameObject* gameObject, float radius, float height);

			CollisionCapsule(GameObject* gameObject, const CollisionCapsule& other);

			void Serialize(Core::SaveElement& parent) override;
			void Deserialize(const  Core::SaveElement& data) override;

			[[nodiscard]] Physic::CollisionCapsule* GetCollisionCapsule() const;

			static void LuaInit();

			void SetRadius(float radius);
			void SetHeight(float height);
			void SetGeometry(float radius, float height);

			float GetRadius() const;
			float GetHeight() const;
		};
	}
}
