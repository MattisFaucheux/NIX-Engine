#include "pch.h"

#include "RigidStatic.h"

#include "Log/Logger.h"

namespace NIX::Physic
{
	RigidStatic::RigidStatic(PxPhysics* physic, PxScene* scene, CollisionShape* shape, const LibMath::Vector3& position) :
		m_collisionShape(shape)
	{
		m_actor = physic->createRigidStatic(PxTransform(position.x, position.y, position.z));
		if (!m_actor)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "Create RigidStatic failed !");
		}

		m_actor->attachShape(*(m_collisionShape->GetShape()));
		scene->addActor(*m_actor);

		m_actor->userData = this;
	}

	RigidStatic::~RigidStatic()
	{
		m_actor->release();
	}

	LibMath::Vector3 RigidStatic::GetPosition() const
	{
		PxVec3 pos = m_actor->getGlobalPose().p;
		return { pos.x, pos.y, pos.z };
	}

	LibMath::Quaternion RigidStatic::GetRotation() const
	{
		PxQuat quat = m_actor->getGlobalPose().q;
		return { quat.x, quat.y, quat.z, quat.w };
	}

	void RigidStatic::SetPosition(const LibMath::Vector3& position)
	{
		PxTransform transform;
		transform.p = { position.x, position.y, position.z };
		transform.q = m_actor->getGlobalPose().q;
		m_actor->setGlobalPose(transform);
	}

	void RigidStatic::SetRotation(const LibMath::Quaternion& quaternion)
	{
		PxTransform transform;
		transform.q = { quaternion.X, quaternion.Y, quaternion.Z, quaternion.W };
		transform.p = m_actor->getGlobalPose().p;
		m_actor->setGlobalPose(transform);
	}

	void RigidStatic::SetTransform(const LibMath::Vector3& position, const LibMath::Quaternion& quaternion)
	{
		PxTransform transform;
		transform.p = { position.x, position.y, position.z };
		transform.q = { quaternion.X, quaternion.Y, quaternion.Z, quaternion.W };
		m_actor->setGlobalPose(transform);
	}

	CollisionShape* RigidStatic::GetCollisionShape() const
	{
		return m_collisionShape;
	}

	void RigidStatic::IsActive(bool isActive)
	{
		m_actor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, !isActive);
	}

	bool RigidStatic::IsActive()
	{
		return !m_actor->getActorFlags().isSet(PxActorFlag::eDISABLE_SIMULATION);
	}

	PxRigidStatic* RigidStatic::GetActor()
	{
		return m_actor;
	}
}
