#include "pch.h"

#include "CollisionSphere.h"

namespace NIX::Physic
{
	CollisionSphere::CollisionSphere(PxPhysics* physic, const PxMaterial& physicMat, float radius) :
		CollisionShape(physic, physicMat, PxSphereGeometry(radius)), m_radius(radius)
	{
	}

	void CollisionSphere::SetRadius(float radius)
	{
		m_radius = radius;
		UpdateGeometry();
	}

	float CollisionSphere::GetRadius() const
	{
		return m_radius;
	}

	void CollisionSphere::UpdateGeometry()
	{
		m_shape->setGeometry(PxSphereGeometry(m_radius));
	}
}

