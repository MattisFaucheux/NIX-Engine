#include "pch.h"

#include "CollisionBox.h"

namespace NIX::Physic
{
	CollisionBox::CollisionBox(PxPhysics* physic, const PxMaterial& physicMat, const LibMath::Vector3& scale) :
		CollisionShape(physic, physicMat, PxBoxGeometry(scale.x / 2, scale.y / 2, scale.z / 2)), m_scale(scale)
	{
	}

	void CollisionBox::SetScaleX(float scaleX)
	{
		m_scale.x = scaleX;
		UpdateGeometry();
	}

	void CollisionBox::SetScaleY(float scaleY)
	{
		m_scale.y = scaleY;
		UpdateGeometry();
	}

	void CollisionBox::SetScaleZ(float scaleZ)
	{
		m_scale.z = scaleZ;
		UpdateGeometry();
	}

	void CollisionBox::SetScale(const LibMath::Vector3& scale)
	{
		m_scale = scale;
		UpdateGeometry();
	}

	float CollisionBox::GetScaleX() const
	{
		return m_scale.x;
	}

	float CollisionBox::GetScaleY() const
	{
		return m_scale.y;
	}

	float CollisionBox::GetScaleZ() const
	{
		return m_scale.z;
	}

	LibMath::Vector3 CollisionBox::GetScale() const
	{
		return m_scale;
	}

	void CollisionBox::SetObjectScale(const LibMath::Vector3& scale)
	{
		m_objectScale = scale;
		UpdateGeometry();
	}

	LibMath::Vector3 CollisionBox::GetObjectScale() const
	{
		return m_objectScale;
	}

	void CollisionBox::UpdateGeometry()
	{
		m_shape->setGeometry(PxBoxGeometry((m_scale.x * m_objectScale.x) / 2, (m_scale.y * m_objectScale.y) / 2, (m_scale.z * m_objectScale.z) / 2));
	}
}
