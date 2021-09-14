#include "pch.h"

#include "RigidBody.h"

#include "Log/Logger.h"

namespace NIX::Physic
{
	RigidBody::RigidBody(PxPhysics* physic, PxScene* scene, CollisionShape* shape, const LibMath::Vector3& position) :
		m_collisionShape(shape)
	{
		m_actor = physic->createRigidDynamic(PxTransform(position.x, position.y, position.z));
		if (!m_actor)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "Create RigidDynamic failed !");
		}

		m_actor->attachShape(*(m_collisionShape->GetShape()));

		PxRigidBodyExt::updateMassAndInertia(*m_actor, 1);
		scene->addActor(*m_actor);

		//m_actor->setActorFlag(PxActorFlag::eSEND_SLEEP_NOTIFIES, true);

		m_actor->userData = this;
	}

	RigidBody::~RigidBody()
	{
		m_actor->release();
	}

	float RigidBody::GetMass() const
	{
		return (float)m_actor->getMass();
	}

	void RigidBody::SetMass(float mass)
	{
		if (mass <= 0)
		{
			mass = 0.001f;
		}
		m_actor->setMass(mass);
	}

	bool RigidBody::GetIsGravity() const
	{
		return !m_actor->getActorFlags().isSet(PxActorFlag::eDISABLE_GRAVITY);
	}

	void RigidBody::SetIsGravity(bool isGravity)
	{
		m_actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !isGravity);
	}

	bool RigidBody::GetIsKinematic() const
	{
		return m_actor->getRigidBodyFlags().isSet(PxRigidBodyFlag::eKINEMATIC);
	}

	void RigidBody::SetIsKinematic(bool isKinematic)
	{
		m_actor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, isKinematic);
	}
	
	void RigidBody::SetIsAxisRotationLock(bool xValue, bool yValue, bool zValue)
	{
		SetIsXRotationLock(xValue);
		SetIsYRotationLock(yValue);
		SetIsZRotationLock(zValue);
	}

	void RigidBody::SetIsAxisTranslationLock(bool xValue, bool yValue, bool zValue)
	{
		SetIsXTranslationLock(xValue);
		SetIsYTranslationLock(yValue);
		SetIsZTranslationLock(zValue);
	}

	void RigidBody::SetIsXRotationLock(bool value)
	{
		m_actor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, value);
	}

	void RigidBody::SetIsYRotationLock(bool value)
	{
		m_actor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, value);
	}

	void RigidBody::SetIsZRotationLock(bool value)
	{
		m_actor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, value);
	}

	void RigidBody::SetIsXTranslationLock(bool value)
	{
		m_actor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_X, value);
	}

	void RigidBody::SetIsYTranslationLock(bool value)
	{
		m_actor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, value);
	}

	void RigidBody::SetIsZTranslationLock(bool value)
	{
		m_actor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, value);
	}

	std::tuple<bool, bool, bool> RigidBody::GetIsAxisRotationLock() const
	{
		return std::make_tuple(GetIsXRotationLock(), GetIsYRotationLock(), GetIsZRotationLock());
	}

	std::tuple<bool, bool, bool> RigidBody::GetIsAxisTranslationLock() const
	{
		return std::make_tuple(GetIsXTranslationLock(), GetIsYTranslationLock(), GetIsZTranslationLock());
	}

	bool RigidBody::GetIsXRotationLock() const
	{
		return m_actor->getRigidDynamicLockFlags().isSet(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X);
	}

	bool RigidBody::GetIsYRotationLock() const
	{
		return m_actor->getRigidDynamicLockFlags().isSet(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y);
	}

	bool RigidBody::GetIsZRotationLock() const
	{
		return m_actor->getRigidDynamicLockFlags().isSet(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);
	}

	bool RigidBody::GetIsXTranslationLock() const
	{
		return m_actor->getRigidDynamicLockFlags().isSet(PxRigidDynamicLockFlag::eLOCK_LINEAR_X);
	}

	bool RigidBody::GetIsYTranslationLock() const
	{
		return m_actor->getRigidDynamicLockFlags().isSet(PxRigidDynamicLockFlag::eLOCK_LINEAR_Y);
	}

	bool RigidBody::GetIsZTranslationLock() const
	{
		return m_actor->getRigidDynamicLockFlags().isSet(PxRigidDynamicLockFlag::eLOCK_LINEAR_Z);
	}

	LibMath::Vector3 RigidBody::GetPosition() const
	{
		PxVec3 pos = m_actor->getGlobalPose().p;
		return { pos.x, pos.y, pos.z };
	}

	LibMath::Quaternion RigidBody::GetRotation() const
	{
		PxQuat quat = m_actor->getGlobalPose().q;
		return { quat.x, quat.y, quat.z, quat.w };
	}

	void RigidBody::SetPosition(const LibMath::Vector3& position, bool isPhysicInteraction)
	{
		PxTransform transform;
		transform.p = { position.x, position.y, position.z };
		transform.q = m_actor->getGlobalPose().q;
		if (isPhysicInteraction)
		{
			m_actor->setKinematicTarget(transform);
		}
		else
		{
			m_actor->setGlobalPose(transform);
		}
	}

	void RigidBody::SetRotation(const LibMath::Quaternion& quaternion, bool isPhysicInteraction)
	{
		PxTransform transform;
		transform.q = { quaternion.X, quaternion.Y, quaternion.Z, quaternion.W };
		transform.p = m_actor->getGlobalPose().p;
		if (isPhysicInteraction)
		{
			m_actor->setKinematicTarget(transform);
		}
		else
		{
			m_actor->setGlobalPose(transform);
		}
	}

	void RigidBody::SetTransform(const LibMath::Vector3& position, const LibMath::Quaternion& quaternion, bool isPhysicInteraction)
	{
		PxTransform transform;
		transform.p = { position.x, position.y, position.z };
		transform.q = { quaternion.X, quaternion.Y, quaternion.Z, quaternion.W };
		if (isPhysicInteraction)
		{
			m_actor->setKinematicTarget(transform);
		}
		else
		{
			m_actor->setGlobalPose(transform);
		}
	}

	CollisionShape* RigidBody::GetCollisionShape() const
	{
		return m_collisionShape;
	}

	float RigidBody::GetLinearDamping() const
	{
		return m_actor->getLinearDamping();
	}


	void RigidBody::SetLinearDamping(float linearDamping)
	{
		m_actor->setLinearDamping(linearDamping);
	}

	float RigidBody::GetAngularDamping() const
	{
		return m_actor->getAngularDamping();
	}

	void RigidBody::SetAngularDamping(float angularDamping)
	{
		m_actor->setAngularDamping(angularDamping);
	}

	float RigidBody::GetMaxLinearVelocity() const
	{
		return m_actor->getMaxLinearVelocity();
	}

	void RigidBody::SetMaxLinearVelocity(float maxLinearVelocity)
	{
		m_actor->setMaxLinearVelocity(maxLinearVelocity);
	}

	float RigidBody::GetMaxAngularVelocity() const
	{
		return m_actor->getMaxAngularVelocity();
	}

	void RigidBody::SetMaxAngularVelocity(float maxAngularVelocity)
	{
		m_actor->setMaxAngularVelocity(maxAngularVelocity);
	}

	LibMath::Vector3 RigidBody::GetAngularVelocity() const
	{
		PxVec3 velocity = m_actor->getAngularVelocity();
		return { velocity.x, velocity.y, velocity.z };
	}

	LibMath::Vector3 RigidBody::GetLinearVelocity() const
	{
		PxVec3 velocity = m_actor->getLinearVelocity();
		return { velocity.x, velocity.y, velocity.z };
	}

	LibMath::Vector3 RigidBody::GetLocalCenterOfMass() const
	{
		PxVec3 pos = m_actor->getCMassLocalPose().p;
		return { pos.x, pos.y, pos.z };
	}

	void RigidBody::SetMassInertiaTensor(const LibMath::Vector3& inertiaTensor)
	{
		m_actor->setMassSpaceInertiaTensor({ inertiaTensor.x, inertiaTensor.y, inertiaTensor.z });
	}

	LibMath::Vector3 RigidBody::GetMassInertiaTensor() const
	{
		PxVec3 tensor = m_actor->getMassSpaceInertiaTensor();
		return { tensor.x, tensor.y, tensor.z };
	}

	void RigidBody::AddForce(const LibMath::Vector3& force, EForceMode forceMode)
	{
		switch (forceMode)
		{
		case EForceMode::FORCE:
			m_actor->addForce({ force.x, force.y, force.z }, PxForceMode::eFORCE);
			break;

		case EForceMode::IMPULSE:
			m_actor->addForce({ force.x, force.y, force.z }, PxForceMode::eIMPULSE);
			break;

		case EForceMode::VELOCITY:
			m_actor->addForce({ force.x, force.y, force.z }, PxForceMode::eVELOCITY_CHANGE);
			break;

		case EForceMode::ACCELERATION:
			m_actor->addForce({ force.x, force.y, force.z }, PxForceMode::eACCELERATION);
			break;

		default:
			m_actor->addForce({ force.x, force.y, force.z }, PxForceMode::eFORCE);
			break;
		}
	}

	void RigidBody::AddForceAtPosition(const LibMath::Vector3& force, const LibMath::Vector3& pos, EForceMode forceMode)
	{
		switch (forceMode)
		{
		case EForceMode::FORCE:
			PxRigidBodyExt::addForceAtPos(*m_actor, { force.x, force.y, force.z }, { pos.x, pos.y, pos.z }, PxForceMode::eFORCE);
			break;

		case EForceMode::IMPULSE:
			PxRigidBodyExt::addForceAtPos(*m_actor, { force.x, force.y, force.z }, { pos.x, pos.y, pos.z }, PxForceMode::eIMPULSE);
			break;

		default:
			PxRigidBodyExt::addForceAtPos(*m_actor, { force.x, force.y, force.z }, { pos.x, pos.y, pos.z }, PxForceMode::eFORCE);
			break;
		}
	}

	void RigidBody::AddLocalForceAtPosition(const LibMath::Vector3& force, const LibMath::Vector3& pos, EForceMode forceMode)
	{
		switch (forceMode)
		{
		case EForceMode::FORCE:
			PxRigidBodyExt::addLocalForceAtPos(*m_actor, { force.x, force.y, force.z }, { pos.x, pos.y, pos.z }, PxForceMode::eFORCE);
			break;

		case EForceMode::IMPULSE:
			PxRigidBodyExt::addLocalForceAtPos(*m_actor, { force.x, force.y, force.z }, { pos.x, pos.y, pos.z }, PxForceMode::eIMPULSE);
			break;

		default:
			PxRigidBodyExt::addLocalForceAtPos(*m_actor, { force.x, force.y, force.z }, { pos.x, pos.y, pos.z }, PxForceMode::eFORCE);
			break;
		}
	}

	void RigidBody::AddForceAtLocalPosition(const LibMath::Vector3& force, const LibMath::Vector3& pos, EForceMode forceMode)
	{
		switch (forceMode)
		{
		case EForceMode::FORCE:
			PxRigidBodyExt::addForceAtLocalPos(*m_actor, { force.x, force.y, force.z }, { pos.x, pos.y, pos.z }, PxForceMode::eFORCE);
			break;

		case EForceMode::IMPULSE:
			PxRigidBodyExt::addForceAtLocalPos(*m_actor, { force.x, force.y, force.z }, { pos.x, pos.y, pos.z }, PxForceMode::eIMPULSE);
			break;

		default:
			PxRigidBodyExt::addForceAtLocalPos(*m_actor, { force.x, force.y, force.z }, { pos.x, pos.y, pos.z }, PxForceMode::eFORCE);
			break;
		}
	}

	void RigidBody::AddLocalForceAtLocalPosition(const LibMath::Vector3& force, const LibMath::Vector3& pos, EForceMode forceMode)
	{
		switch (forceMode)
		{
		case EForceMode::FORCE:
			PxRigidBodyExt::addLocalForceAtLocalPos(*m_actor, { force.x, force.y, force.z }, { pos.x, pos.y, pos.z }, PxForceMode::eFORCE);
			break;

		case EForceMode::IMPULSE:
			PxRigidBodyExt::addLocalForceAtLocalPos(*m_actor, { force.x, force.y, force.z }, { pos.x, pos.y, pos.z }, PxForceMode::eIMPULSE);
			break;

		default:
			PxRigidBodyExt::addLocalForceAtLocalPos(*m_actor, { force.x, force.y, force.z }, { pos.x, pos.y, pos.z }, PxForceMode::eFORCE);
			break;
		}
	}

	void RigidBody::AddTorque(const LibMath::Vector3& torque, EForceMode forceMode)
	{
		switch (forceMode)
		{
		case EForceMode::FORCE:
			m_actor->addTorque({ torque.x, torque.y, torque.z }, PxForceMode::eFORCE);
			break;

		case EForceMode::IMPULSE:
			m_actor->addTorque({ torque.x, torque.y, torque.z }, PxForceMode::eIMPULSE);
			break;

		case EForceMode::VELOCITY:
			m_actor->addTorque({ torque.x, torque.y, torque.z }, PxForceMode::eVELOCITY_CHANGE);
			break;

		case EForceMode::ACCELERATION:
			m_actor->addTorque({ torque.x, torque.y, torque.z }, PxForceMode::eACCELERATION);
			break;

		default:
			m_actor->addTorque({ torque.x, torque.y, torque.z }, PxForceMode::eFORCE);
			break;
		}
	}

	void RigidBody::ClearForce()
	{
		m_actor->clearForce(PxForceMode::eFORCE);
		m_actor->clearForce(PxForceMode::eACCELERATION);
		m_actor->clearForce(PxForceMode::eIMPULSE);
		m_actor->clearForce(PxForceMode::eVELOCITY_CHANGE);
	}

	void RigidBody::ClearTorque()
	{
		m_actor->clearTorque(PxForceMode::eFORCE);
		m_actor->clearTorque(PxForceMode::eACCELERATION);
		m_actor->clearTorque(PxForceMode::eIMPULSE);
		m_actor->clearTorque(PxForceMode::eVELOCITY_CHANGE);
	}

	void RigidBody::ClearVelocity()
	{
		m_actor->setAngularVelocity({ 0, 0, 0 });
		m_actor->setLinearVelocity({ 0, 0, 0 });
	}

	bool RigidBody::GetIsSleeping()
	{
		return m_actor->isSleeping();
	}

	void RigidBody::WakeUp()
	{
		m_actor->wakeUp();
	}

	void RigidBody::PutToSleep()
	{
		m_actor->putToSleep();
	}

	PxRigidDynamic* RigidBody::GetActor()
	{
		return m_actor;
	}

	void RigidBody::IsActive(bool isActive)
	{
		m_actor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, !isActive);
	}

	bool RigidBody::IsActive()
	{
		return !m_actor->getActorFlags().isSet(PxActorFlag::eDISABLE_SIMULATION);
	}
}
