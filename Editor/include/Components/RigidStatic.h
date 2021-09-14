#pragma once

#include "Component.h"
#include "GameObject.h"

#include "Vector/Vector3.h"
#include "Quaternion/Quaternion.h"

namespace NIX
{
	namespace Physic
	{
		class RigidStatic;
	}

	namespace Editor
	{
		class RigidStatic : public Component
		{
		public:
			RigidStatic(GameObject* gameObject);
			
			RigidStatic(GameObject* gameObject, const LibMath::Vector3& position, Physic::CollisionShape* collisionShape);

			RigidStatic(GameObject* gameObject, const RigidStatic& other);

			~RigidStatic();

			void Serialize(Core::SaveElement& parent) override;
			void Deserialize(const Core::SaveElement& data) override;

			void SetIsActive(bool value) override;

			[[nodiscard]] Physic::RigidStatic* GetRigidStatic();

			void SetPosition(const LibMath::Vector3& position);
			void SetRotation(const LibMath::Quaternion& quaternion);

			static void LuaInit();

		private:
			Physic::RigidStatic* m_rigidStatic = nullptr;
		};
	}
}
