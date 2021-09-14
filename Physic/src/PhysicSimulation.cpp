#include "pch.h"

#include "PhysicSimulation.h"

#include <cassert>
#include <iostream>
#include <PxPhysicsVersion.h>

#include "CollisionMesh.h"
#include "CollisionCapsule.h"
#include "CollisionSphere.h"
#include "CollisionBox.h"

#include "PhysicSimulationFilterShader.h"
#include "Log/Logger.h"
using namespace physx;

static PxDefaultErrorCallback gDefaultErrorCallback;
static PxDefaultAllocator gDefaultAllocatorCallback;


namespace NIX::Physic
{
	PhysicSimulation::PhysicSimulation(bool usePvd) :
		m_usePvd(usePvd)
	{
		
		if (m_singleton != nullptr)
		{			
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_WARNING, "An other instance of the physic simulation singleton tried to be created");
			return;
		}

		m_singleton = this;
		
		//Create PxFoundation*
		m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);

		if (!m_foundation)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "PxCreateFoundation failed !");
		}

		//Create PxToleranceScale
		m_tolerancesScale.length = 100;
		m_tolerancesScale.speed = 981;

		if (m_usePvd)
		{
			//Create PxPvd*
			m_pvd = PxCreatePvd(*m_foundation);
			m_transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
			m_pvd->connect(*m_transport, PxPvdInstrumentationFlag::eALL);

			//Create PxPhysics*
			m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, m_tolerancesScale, m_recordMemoryAllocations, m_pvd);
			if (!m_physics) 
			{
				LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "PxCreatePhysics failed !");
			}

		}
		else
		{
			//Create PxPhysics*
			m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, m_tolerancesScale, m_recordMemoryAllocations);
			if (!m_physics)
			{
				LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "PxCreatePhysics failed !");
			}
		}

		//Create PxCooking*
		m_cooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_foundation, PxCookingParams(m_tolerancesScale));
		if (!m_cooking)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "PxCreateCooking failed !");
		}

		m_dispatcher = PxDefaultCpuDispatcherCreate(m_nbThreads);
		
	}

	PhysicSimulation::~PhysicSimulation()
	{
		m_currentScene->GetScene()->release();
		m_dispatcher->release();
		m_cooking->release();
		m_physics->release();
		if (m_usePvd)
		{
			m_pvd->release();
			m_transport->release();
		}
		m_foundation->release();
	}

	PHYSIC_LIBRARY_API bool PhysicSimulation::UpdateSimulation(float deltaTime)
	{
		if (m_currentScene == nullptr)
		{
			//LOG(SOURCE_INFO, Core::EVerbosity::LOG_INFO, "No physic simulation is set as current");
			return false;
		}
		
		m_accumulator += deltaTime;
		if (m_accumulator < m_stepSize)
		{
			return false;
		}

		m_accumulator -= m_stepSize;

		m_currentScene->GetScene()->simulate(m_stepSize);
		m_currentScene->GetScene()->fetchResults(true);

		return true;
	}

	void PhysicSimulation::CreateMaterial(float staticFriction, float dynamicFriction, float restitution)
	{
		m_material = m_physics->createMaterial(staticFriction, dynamicFriction, restitution);
		if (!m_material)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "CreateMaterial failed !");
		}
	}

	CollisionBox* PhysicSimulation::CreateCollisionBox(const LibMath::Vector3& scale)
	{
		CollisionBox* cs = new CollisionBox(m_physics, *m_material, scale);
		return cs;
	}

	CollisionCapsule* PhysicSimulation::CreateCollisionCapsule(float radius, float height)
	{
		CollisionCapsule* cs = new CollisionCapsule(m_physics, *m_material, radius, height);
		return cs;
	}

	CollisionSphere* PhysicSimulation::CreateCollisionSphere(float radius)
	{
		CollisionSphere* cs = new CollisionSphere(m_physics, *m_material, radius);
		return cs;
	}

	CollisionMesh* PhysicSimulation::CreateCollisionMesh(const std::vector<LibMath::Vector3>& vertex, const LibMath::Vector3& scale)
	{		
		CollisionMesh* collisionMesh = new CollisionMesh(m_physics, *m_material, CreateConvexMesh(vertex), vertex, scale);

		return collisionMesh;
	}

	RigidBody* PhysicSimulation::CreateRigidBody(const LibMath::Vector3& position, CollisionShape* shape)
	{
		if (!shape)
		{
			return nullptr;
		}

		RigidBody* rb = new RigidBody(m_physics, m_currentScene->GetScene(), shape, position);
		return rb;
	}

	RigidStatic* PhysicSimulation::CreateRigidStatic(const LibMath::Vector3& position, CollisionShape* shape)
	{
		if (!shape)
		{
			return nullptr;
		}

		RigidStatic* rs = new RigidStatic(m_physics, m_currentScene->GetScene(), shape, position);
		return rs;
	}

	void PhysicSimulation::SetGravity(const LibMath::Vector3& gravity)
	{
		m_currentScene->GetScene()->setGravity({ gravity.x, gravity.y, gravity.z });
	}

	LibMath::Vector3 PhysicSimulation::GetGravity()
	{
		PxVec3 gravity = m_currentScene->GetScene()->getGravity();
		return { gravity.x, gravity.y, gravity.z };
	}


	physx::PxPhysics* PhysicSimulation::GetPhysics()
	{
		return m_physics;
	}

	PhysicSimulationCallback& PhysicSimulation::GetSimulationCallback()
	{
		return m_simulationCallback;
	}

	physx::PxDefaultCpuDispatcher* PhysicSimulation::GetDispatcher() const
	{
		return m_dispatcher;
	}

	const std::vector<LibMath::Vector3>& PhysicSimulation::GetDebugPoints()
	{
		m_debugPoints.clear();

		const PxRenderBuffer& rb = m_currentScene->GetScene()->getRenderBuffer();
		for (PxU32 i = 0; i < rb.getNbPoints(); i++)
		{
			const PxDebugPoint& point = rb.getPoints()[i];
			m_debugPoints.emplace_back( point.pos.x, point.pos.y, point.pos.z);
		}

		return m_debugPoints;
	}

	const std::vector<LibMath::Vector3>& PhysicSimulation::GetDebugLines()
	{
		m_debugPoints.clear();

		const PxRenderBuffer& rb = m_currentScene->GetScene()->getRenderBuffer();
		for (PxU32 i = 0; i < rb.getNbLines(); i++)
		{
			const PxDebugLine& line = rb.getLines()[i];
			m_debugPoints.emplace_back(line.pos0.x, line.pos0.y, line.pos0.z);
			m_debugPoints.emplace_back(line.pos1.x, line.pos1.y, line.pos1.z);
		}

		return m_debugPoints;
	}

	const std::vector<LibMath::Vector3>& PhysicSimulation::GetDebugTriangles()
	{
		m_debugPoints.clear();

		const PxRenderBuffer& rb = m_currentScene->GetScene()->getRenderBuffer();
		for (PxU32 i = 0; i < rb.getNbTriangles(); i++)
		{
			const PxDebugTriangle& triangle = rb.getTriangles()[i];
			m_debugPoints.emplace_back(triangle.pos0.x, triangle.pos0.y, triangle.pos0.z);
			m_debugPoints.emplace_back(triangle.pos1.x, triangle.pos1.y, triangle.pos1.z);
			m_debugPoints.emplace_back(triangle.pos2.x, triangle.pos2.y, triangle.pos2.z);
		}

		return m_debugPoints;
	}

	void PhysicSimulation::SetCurrentScene(PhysicScene* currentScene)
	{
		m_currentScene = currentScene;
	}

	PhysicScene* PhysicSimulation::GetCurrentScene()
	{
		return m_currentScene;
	}

	PhysicSimulation* PhysicSimulation::GetSingleton()
	{
		return m_singleton;
	}

	PxConvexMesh* PhysicSimulation::CreateConvexMesh(const std::vector<LibMath::Vector3>& vertex) const
	{
		PxConvexMeshDesc convexDesc;
		convexDesc.points.count = static_cast<PxU32>(vertex.size());
		convexDesc.points.stride = sizeof(LibMath::Vector3);
		convexDesc.points.data = vertex.data();
		convexDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;
		PxDefaultMemoryOutputStream buf;
		PxConvexMeshCookingResult::Enum result;
		
		if (!m_cooking->cookConvexMesh(convexDesc, buf, &result))
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "Create shape failed !");
		}
		
		PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
		PxConvexMesh* convexMesh = m_physics->createConvexMesh(input);
		return convexMesh;
	}
}
