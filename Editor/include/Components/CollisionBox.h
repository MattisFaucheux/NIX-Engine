#pragma once

#include "Components/CollisionShapeComponent.h"
#include "GameObject.h"

namespace NIX
{
	namespace Physic
	{
		class CollisionBox;
	}

	namespace Editor
	{
		class CollisionBox : public CollisionShapeComponent
		{
		public:
			CollisionBox(GameObject* gameObject);
			CollisionBox(GameObject* gameObject, const LibMath::Vector3& scale);

			CollisionBox(GameObject* gameObject, const CollisionBox& other);


			void Serialize(Core::SaveElement& parent) override;
			void Deserialize(const Core::SaveElement& data) override;

			[[nodiscard]] Physic::CollisionBox* GetCollisionBox() const;

			static void LuaInit();

			void SetScaleX(float scaleX);
			void SetScaleY(float scaleY);
			void SetScaleZ(float scaleZ);
			void SetScale(LibMath::Vector3& scale);

			[[nodiscard]] float GetScaleX() const;
			[[nodiscard]] float GetScaleY() const;
			[[nodiscard]] float GetScaleZ() const;
			[[nodiscard]] LibMath::Vector3 GetScale() const;
		};
	}
}
