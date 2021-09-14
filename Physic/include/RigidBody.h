#pragma once
#include "DLLExport.h"

#include <PxPhysicsAPI.h>

#include "Vector/Vector3.h"
#include "Quaternion/Quaternion.h"

#include "CollisionShape.h"

#include "ForceMode.h"

#include <tuple>

namespace NIX::Physic
{
	using namespace physx;

	class RigidBody
	{
	public:

		RigidBody() = delete;
		RigidBody(PxPhysics* physic, PxScene* scene, CollisionShape* shape, const LibMath::Vector3& position);
		RigidBody(const RigidBody& other) = delete;
		RigidBody(RigidBody&& other) noexcept = delete;
		RigidBody& operator=(const RigidBody& other) = delete;
		RigidBody& operator=(RigidBody&& other) noexcept = delete;
		PHYSIC_LIBRARY_API ~RigidBody();

		//Set Mass
		PHYSIC_LIBRARY_API [[nodiscard]] float GetMass() const;
		PHYSIC_LIBRARY_API void SetMass(float mass);

		//Enable / Disable Gravity
		PHYSIC_LIBRARY_API [[nodiscard]] bool GetIsGravity() const;
		PHYSIC_LIBRARY_API void SetIsGravity(bool isGravity);

		//Enable / Disable Kinematic
		PHYSIC_LIBRARY_API [[nodiscard]] bool GetIsKinematic() const;
		PHYSIC_LIBRARY_API void SetIsKinematic(bool isKinematic);

		//Enable/Disable Axis Rotation
		PHYSIC_LIBRARY_API [[nodiscard]] std::tuple<bool, bool, bool> GetIsAxisRotationLock() const;
		PHYSIC_LIBRARY_API [[nodiscard]] bool GetIsXRotationLock() const;
		PHYSIC_LIBRARY_API [[nodiscard]] bool GetIsYRotationLock() const;
		PHYSIC_LIBRARY_API [[nodiscard]] bool GetIsZRotationLock() const;
		PHYSIC_LIBRARY_API void SetIsAxisRotationLock(bool xValue, bool yValue, bool zValue);
		PHYSIC_LIBRARY_API void SetIsXRotationLock(bool value);
		PHYSIC_LIBRARY_API void SetIsYRotationLock(bool value);
		PHYSIC_LIBRARY_API void SetIsZRotationLock(bool value);

		//Enable/Disable Axis Translation
		PHYSIC_LIBRARY_API [[nodiscard]] std::tuple<bool, bool, bool> GetIsAxisTranslationLock() const;
		PHYSIC_LIBRARY_API [[nodiscard]] bool GetIsXTranslationLock() const;
		PHYSIC_LIBRARY_API [[nodiscard]] bool GetIsYTranslationLock() const;
		PHYSIC_LIBRARY_API [[nodiscard]] bool GetIsZTranslationLock() const;
		PHYSIC_LIBRARY_API void SetIsAxisTranslationLock(bool xValue, bool yValue, bool zValue);
		PHYSIC_LIBRARY_API void SetIsXTranslationLock(bool value);
		PHYSIC_LIBRARY_API void SetIsYTranslationLock(bool value);
		PHYSIC_LIBRARY_API void SetIsZTranslationLock(bool value);

		//Get RigidBody Position
		PHYSIC_LIBRARY_API [[nodiscard]] LibMath::Vector3 GetPosition() const;
		//Get Rigidbody Rotation
		PHYSIC_LIBRARY_API [[nodiscard]] LibMath::Quaternion GetRotation() const;

		PHYSIC_LIBRARY_API void SetPosition(const LibMath::Vector3& position, bool isPhysicInteraction = false);
		PHYSIC_LIBRARY_API void SetRotation(const LibMath::Quaternion& quaternion, bool isPhysicInteraction = false);
		PHYSIC_LIBRARY_API void SetTransform(const LibMath::Vector3& position, const LibMath::Quaternion& quaternion, bool isPhysicInteraction = false);

		//Get Collision Shape
		PHYSIC_LIBRARY_API [[nodiscard]] CollisionShape* GetCollisionShape() const;

		//Set Linear Dampling
		PHYSIC_LIBRARY_API [[nodiscard]] float GetLinearDamping() const;
		PHYSIC_LIBRARY_API void SetLinearDamping(float linearDamping);

		//Set Angular Dampling
		PHYSIC_LIBRARY_API [[nodiscard]] float GetAngularDamping() const;
		PHYSIC_LIBRARY_API void SetAngularDamping(float angularDamping);

		//Set Max Linear Velocity
		PHYSIC_LIBRARY_API [[nodiscard]] float GetMaxLinearVelocity() const;
		PHYSIC_LIBRARY_API void SetMaxLinearVelocity(float maxLinearVelocity);

		//Set Max Angular Velocity
		PHYSIC_LIBRARY_API [[nodiscard]] float GetMaxAngularVelocity() const;
		PHYSIC_LIBRARY_API void SetMaxAngularVelocity(float maxAngularVelocity);

		//Get Velocity
		PHYSIC_LIBRARY_API [[nodiscard]] LibMath::Vector3 GetAngularVelocity() const;
		PHYSIC_LIBRARY_API [[nodiscard]] LibMath::Vector3 GetLinearVelocity() const;

		//Get Center Of Mass
		PHYSIC_LIBRARY_API [[nodiscard]] LibMath::Vector3 GetLocalCenterOfMass() const;

		PHYSIC_LIBRARY_API void SetMassInertiaTensor(const LibMath::Vector3& inertiaTensor);
		PHYSIC_LIBRARY_API [[nodiscard]] LibMath::Vector3 GetMassInertiaTensor() const;

		//Add Force
		PHYSIC_LIBRARY_API void AddForce(const LibMath::Vector3& force, EForceMode forceMode = EForceMode::FORCE);
		PHYSIC_LIBRARY_API void AddForceAtPosition(const LibMath::Vector3& force, const LibMath::Vector3& pos, EForceMode forceMode = EForceMode::FORCE);
		PHYSIC_LIBRARY_API void AddLocalForceAtPosition(const LibMath::Vector3& force, const LibMath::Vector3& pos, EForceMode forceMode = EForceMode::FORCE);
		PHYSIC_LIBRARY_API void AddForceAtLocalPosition(const LibMath::Vector3& force, const LibMath::Vector3& pos, EForceMode forceMode = EForceMode::FORCE);
		PHYSIC_LIBRARY_API void AddLocalForceAtLocalPosition(const LibMath::Vector3& force, const LibMath::Vector3& pos, EForceMode forceMode = EForceMode::FORCE);

		//Add Torque
		PHYSIC_LIBRARY_API void AddTorque(const LibMath::Vector3& torque, EForceMode forceMode = EForceMode::FORCE);

		PHYSIC_LIBRARY_API void ClearForce();
		PHYSIC_LIBRARY_API void ClearTorque();
		PHYSIC_LIBRARY_API void ClearVelocity();
		
		PHYSIC_LIBRARY_API bool GetIsSleeping();
		PHYSIC_LIBRARY_API void WakeUp();
		PHYSIC_LIBRARY_API void PutToSleep();

		[[nodiscard]] PxRigidDynamic* GetActor();

		PHYSIC_LIBRARY_API void IsActive(bool isActive);
		PHYSIC_LIBRARY_API bool IsActive();
		
	private:

		PxRigidDynamic* m_actor = nullptr;
		CollisionShape* m_collisionShape = nullptr;
	};
}
