#include "pch.h"

#include "CollisionMesh.h"
#include "PhysicSimulation.h"

namespace NIX::Physic
{
	CollisionMesh::CollisionMesh(PxPhysics* physic, const PxMaterial& physicMat, PxConvexMesh* convexMesh, const std::vector<LibMath::Vector3>& points, const LibMath::Vector3& scale) :
		CollisionShape(physic, physicMat, PxConvexMeshGeometry(convexMesh, PxMeshScale({scale.x, scale.y, scale.z}))), m_convexMesh(convexMesh), m_scale(scale), m_points(points)
	{}

	void CollisionMesh::SetGeometry(PxConvexMesh* convexMesh)
	{
		m_convexMesh->release();
		m_convexMesh = convexMesh;
		UpdateGeometry();
	}

	void CollisionMesh::SetGeometry(const std::vector<LibMath::Vector3>& points, const LibMath::Vector3& scale)
	{
		m_scale = scale;
		m_points = points;

		m_convexMesh->release();
		m_convexMesh = PhysicSimulation::GetSingleton()->CreateConvexMesh(m_points);
		UpdateGeometry();
	}

	void CollisionMesh::SetScale(const LibMath::Vector3& scale)
	{
		if (m_scale == scale)
		{
			return;
		}
		
		m_scale = scale;

		m_convexMesh->release();
		m_convexMesh = PhysicSimulation::GetSingleton()->CreateConvexMesh(m_points);
		UpdateGeometry();
	}

	const LibMath::Vector3& CollisionMesh::GetScale() const
	{
		return m_scale;
	}

	void CollisionMesh::SetPoints(const std::vector<LibMath::Vector3>& points)
	{
		m_points = points;

		m_convexMesh->release();
		m_convexMesh = PhysicSimulation::GetSingleton()->CreateConvexMesh(m_points);
		UpdateGeometry();
	}

	const std::vector<LibMath::Vector3>& CollisionMesh::GetPoints() const
	{

		return m_points;
	}

	void CollisionMesh::UpdateGeometry()
	{		
		m_shape->setGeometry(PxConvexMeshGeometry(m_convexMesh, PxMeshScale({ m_scale.x, m_scale.y, m_scale.z })));
	}
}
