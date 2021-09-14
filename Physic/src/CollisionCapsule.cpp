#include "pch.h"

#include "CollisionCapsule.h"

namespace NIX::Physic
{
	CollisionCapsule::CollisionCapsule(PxPhysics* physic, const PxMaterial& physicMat, float radius, float height) :
		CollisionShape(physic, physicMat, PxCapsuleGeometry(radius, height/2)), m_radius(radius), m_height(height)
	{
	}

	void CollisionCapsule::SetRadius(float radius)
	{
		m_radius = radius;
		UpdateGeometry();
	}

	void CollisionCapsule::SetHeight(float height)
	{
		m_height = height;
		UpdateGeometry();
	}

	void CollisionCapsule::SetGeometry(float radius, float height)
	{
		m_radius = radius;
		m_height = height;
		UpdateGeometry();
	}

	float CollisionCapsule::GetRadius() const
	{
		return m_radius;
	}

	float CollisionCapsule::GetHeight() const
	{
		return m_height;
	}

	std::pair<float, float> CollisionCapsule::GetRadiusAndHeight() const
	{
		return std::make_pair(m_radius, m_height);
	}

	void CollisionCapsule::UpdateGeometry()
	{
		m_shape->setGeometry(PxCapsuleGeometry(m_radius, m_height / 2));
	}
}
