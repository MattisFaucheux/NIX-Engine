#pragma once

#include "Component.h"
#include "GameObject.h"

#include "Vector/Vector3.h"
#include "Quaternion/Quaternion.h"

namespace NIX
{
	namespace Physic
	{
		class RigidBody;
	}

	namespace Editor
	{
		class RigidBody : public Component
		{
		public:
			RigidBody(GameObject* gameObject);
			
			RigidBody(GameObject* gameObject, const LibMath::Vector3& position, Physic::CollisionShape* collisionShape);

			RigidBody(GameObject* gameObject, const RigidBody& other);

			~RigidBody();

			void Serialize(Core::SaveElement& parent) override;
			void Deserialize(const Core::SaveElement& data) override;

			void SetIsActive(bool value) override;

			[[nodiscard]] Physic::RigidBody* GetRigidBody() const;

			static void LuaInit();


			void SetPosition(const LibMath::Vector3& position);
			void SetRotation(const LibMath::Quaternion& quaternion);


			[[nodiscard]] float GetMass() const;
			void SetMass(float mass);

			[[nodiscard]] bool GetIsGravity() const;
			void SetIsGravity(bool isGravity);

			[[nodiscard]] bool GetIsKinematic() const;
			void SetIsKinematic(bool isKinematic);

			[[nodiscard]] bool GetIsXRotationLock() const;
			[[nodiscard]] bool GetIsYRotationLock() const;
			[[nodiscard]] bool GetIsZRotationLock() const;
			void SetIsXRotationLock(bool value);
			void SetIsYRotationLock(bool value);
			void SetIsZRotationLock(bool value);

			[[nodiscard]] bool GetIsXTranslationLock() const;
			[[nodiscard]] bool GetIsYTranslationLock() const;
			[[nodiscard]] bool GetIsZTranslationLock() const;
			void SetIsXTranslationLock(bool value);
			void SetIsYTranslationLock(bool value);
			void SetIsZTranslationLock(bool value);

			[[nodiscard]] float GetLinearDamping() const;
			void SetLinearDamping(float linearDamping);

			[[nodiscard]] float GetAngularDamping() const;
			void SetAngularDamping(float angularDamping);

			[[nodiscard]] float GetMaxLinearVelocity() const;
			void SetMaxLinearVelocity(float maxLinearVelocity);

			[[nodiscard]] float GetMaxAngularVelocity() const;
			void SetMaxAngularVelocity(float maxAngularVelocity);

			[[nodiscard]] LibMath::Vector3 GetAngularVelocity() const;
			[[nodiscard]] LibMath::Vector3 GetLinearVelocity() const;

			[[nodiscard]] LibMath::Vector3 GetLocalCenterOfMass() const;

			void SetMassInertiaTensor(const LibMath::Vector3& inertiaTensor);
			[[nodiscard]] LibMath::Vector3 GetMassInertiaTensor() const;

			void ClearForce();
			void ClearTorque();
			void ClearVelocity();

			bool GetIsSleeping();
			void WakeUp();
			void PutToSleep();

		private:

			void AddForce(const LibMath::Vector3& force, std::string forceMode = "Force");
			void AddForceAtPosition(const LibMath::Vector3& force, const LibMath::Vector3& pos, std::string forceMode = "Force");
			void AddLocalForceAtPosition(const LibMath::Vector3& force, const LibMath::Vector3& pos, std::string forceMode = "Force");
			void AddForceAtLocalPosition(const LibMath::Vector3& force, const LibMath::Vector3& pos, std::string forceMode = "Force");
			void AddLocalForceAtLocalPosition(const LibMath::Vector3& force, const LibMath::Vector3& pos, std::string forceMode = "Force");

			void AddTorque(const LibMath::Vector3& torque, std::string forceMode = "Force");

			Physic::RigidBody* m_rigidBody = nullptr;
		};
	}
}
