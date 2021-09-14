#include "pch.h"

#include "CollisionShape.h"

#include "Log/Logger.h"

namespace NIX::Physic
{
	CollisionShape::CollisionShape(PxPhysics* physic, const PxMaterial& physicMat, const PxBoxGeometry& shapeGeometry)
	{
		m_shape = physic->createShape(shapeGeometry, physicMat, true);
		if (!m_shape)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "Create shape failed!");
		}

		m_shape->setFlag(PxShapeFlag::eVISUALIZATION, false);

		m_shape->userData = this;
	}

	CollisionShape::CollisionShape(PxPhysics* physic, const PxMaterial& physicMat, const PxCapsuleGeometry& shapeGeometry)
	{
		m_shape = physic->createShape(shapeGeometry, physicMat, true);
		if (!m_shape)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "create shape failed !");
		}

		PxTransform relativePose(PxQuat(PxHalfPi, PxVec3(0, 0, 1)));
		m_shape->setLocalPose(relativePose);

		m_shape->setFlag(PxShapeFlag::eVISUALIZATION, false);

		m_shape->userData = this;
	}

	CollisionShape::CollisionShape(PxPhysics* physic, const PxMaterial& physicMat, const PxConvexMeshGeometry& shapeGeometry)
	{
		m_shape = physic->createShape(shapeGeometry, physicMat, true);
		if (!m_shape)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "Create shape failed!");
		}

		m_shape->setFlag(PxShapeFlag::eVISUALIZATION, false);

		m_shape->userData = this;
	}

	CollisionShape::CollisionShape(PxPhysics* physic, const PxMaterial& physicMat, const PxSphereGeometry& shapeGeometry)
	{
		m_shape = physic->createShape(shapeGeometry, physicMat, true);
		if (!m_shape)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "Create shape failed!");
		}

		m_shape->setFlag(PxShapeFlag::eVISUALIZATION, false);

		m_shape->userData = this;
	}

	CollisionShape::~CollisionShape()
	{
		m_shape->release();
	}

	//void CollisionShape::SetIsContactTestEnable(bool isEnable)
	//{
	//	if (GetIsTrigger())
	//	{
	//		SetIsTrigger(false);
	//	}
	//	m_shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, isEnable);
	//}

	//bool CollisionShape::GetIsContactTestEnable()
	//{
	//	return m_shape->getFlags().isSet(PxShapeFlag::eSIMULATION_SHAPE);
	//}

	void CollisionShape::SetIsQueryTestEnable(bool isEnable)
	{
		m_shape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, isEnable);
	}

	bool CollisionShape::GetIsQueryTestEnable() const
	{
		return m_shape->getFlags().isSet(PxShapeFlag::eSCENE_QUERY_SHAPE);
	}

	void CollisionShape::SetIsTrigger(bool isTrigger)
	{
		if (isTrigger)
		{
			m_shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !isTrigger);
			m_shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, isTrigger);
		}
		else
		{
			m_shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, isTrigger);
			m_shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !isTrigger);
		}
		
	}

	bool CollisionShape::GetIsTrigger() const
	{
		return m_shape->getFlags().isSet(PxShapeFlag::eTRIGGER_SHAPE);
	}

	void CollisionShape::IsDebugDraw(bool isDebugDraw)
	{
		m_shape->setFlag(PxShapeFlag::eVISUALIZATION, isDebugDraw);
	}

	bool CollisionShape::IsDebugDraw() const
	{
		return m_shape->getFlags().isSet(PxShapeFlag::eVISUALIZATION);
	}

	physx::PxShape* CollisionShape::GetShape()
	{
		return m_shape;
	}
	void CollisionShape::OnCollision(CollisionShape* otherShape, bool collisionBegin)
	{
		if (m_callbackCollisionFunction && otherShape)
		{
			m_callbackCollisionFunction(otherShape, collisionBegin);
		}
	}
	void CollisionShape::OnTrigger(CollisionShape* otherShape, bool triggerEnter)
	{
		if (m_callbackTriggerFunction && otherShape)
		{
			m_callbackTriggerFunction(otherShape, triggerEnter);
		}
	}
}
