#pragma once

#include "Components/CollisionShapeComponent.h"
#include "GameObject.h"

namespace NIX
{
	namespace Physic
	{
		class CollisionMesh;
	}

	namespace Editor
	{
		class CollisionMesh : public CollisionShapeComponent
		{
		public:
			CollisionMesh(GameObject* gameObject);
			CollisionMesh(GameObject* gameObject, const std::vector<LibMath::Vector3>& points, const LibMath::Vector3& scale);

			CollisionMesh(GameObject* gameObject, const CollisionMesh& other);

			void Serialize(Core::SaveElement& parent) override;
			void Deserialize(const Core::SaveElement& data) override;

			void SetPoints(const std::vector<LibMath::Vector3>& points);

			[[nodiscard]] Physic::CollisionMesh* GetCollisionMesh() const;

			static void LuaInit();
		};
	}
}
