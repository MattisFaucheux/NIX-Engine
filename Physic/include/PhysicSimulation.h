#pragma once
#include "DLLExport.h"

#include <PxPhysicsAPI.h>
#include <vector>

#include "RigidBody.h"
#include "RigidStatic.h"
#include "CollisionBox.h"
#include "CollisionCapsule.h"
#include "CollisionMesh.h"
#include "CollisionSphere.h"
#include "PhysicSimulationCallback.h"

#include "PhysicScene.h"

#include "Vector/Vector3.h"

namespace NIX::Physic
{
	using namespace physx;
	
	class PhysicSimulation
	{
	public:

		PHYSIC_LIBRARY_API PhysicSimulation(bool usePvd = false);
		PhysicSimulation(const PhysicSimulation& other) = delete;
		PhysicSimulation(PhysicSimulation&& other) noexcept = delete;
		PhysicSimulation& operator=(const PhysicSimulation& other) = delete;
		PhysicSimulation& operator=(PhysicSimulation&& other) noexcept = delete;
		PHYSIC_LIBRARY_API ~PhysicSimulation();


		PHYSIC_LIBRARY_API bool UpdateSimulation(float deltaTime);

		PHYSIC_LIBRARY_API void CreateMaterial(float staticFriction, float dynamicFriction, float restitution);

		PHYSIC_LIBRARY_API CollisionBox* CreateCollisionBox(const LibMath::Vector3& scale);
		PHYSIC_LIBRARY_API CollisionCapsule* CreateCollisionCapsule(float radius, float height);
		PHYSIC_LIBRARY_API CollisionSphere* CreateCollisionSphere(float radius);
		PHYSIC_LIBRARY_API CollisionMesh* CreateCollisionMesh(const std::vector<LibMath::Vector3>& vertex, const LibMath::Vector3& scale);

		PHYSIC_LIBRARY_API RigidBody* CreateRigidBody(const LibMath::Vector3& position, CollisionShape* shape);

		PHYSIC_LIBRARY_API RigidStatic* CreateRigidStatic(const LibMath::Vector3& position, CollisionShape* shape);

		PHYSIC_LIBRARY_API void SetGravity(const LibMath::Vector3& gravity);
		PHYSIC_LIBRARY_API [[nodiscard]] LibMath::Vector3 GetGravity();

		[[nodiscard]] physx::PxPhysics* GetPhysics();
		[[nodiscard]] physx::PxDefaultCpuDispatcher* GetDispatcher() const;
		[[nodiscard]] PhysicSimulationCallback& GetSimulationCallback();
		
		PHYSIC_LIBRARY_API [[nodiscard]] const std::vector<LibMath::Vector3>& GetDebugPoints();
		PHYSIC_LIBRARY_API [[nodiscard]] const std::vector<LibMath::Vector3>& GetDebugLines();
		PHYSIC_LIBRARY_API [[nodiscard]] const std::vector<LibMath::Vector3>& GetDebugTriangles();

		PHYSIC_LIBRARY_API void SetCurrentScene(PhysicScene* currentScene);
		[[nodiscard]] PhysicScene* GetCurrentScene();

		PHYSIC_LIBRARY_API [[nodiscard]] static PhysicSimulation* GetSingleton();

		PxConvexMesh* CreateConvexMesh(const std::vector<LibMath::Vector3>& vertex) const;

	private:

		inline static PhysicSimulation* m_singleton = nullptr;

		PxFoundation* m_foundation;
		PxPvd* m_pvd;
		PxPvdTransport* m_transport;
		PxPhysics* m_physics;
		PxCooking* m_cooking;
		PxDefaultCpuDispatcher* m_dispatcher;
		PhysicScene* m_currentScene = nullptr;

		PhysicSimulationCallback m_simulationCallback;

		PxTolerancesScale m_tolerancesScale;
		PxReal m_objectsAverageLength = 100;
		PxReal m_objectsAverageSpeed = 981;
		bool m_recordMemoryAllocations = true;
		int m_nbThreads = 1;

		PxReal m_accumulator = 0.0f;
		PxReal m_stepSize = 1.0f / 60.0f;

		PxMaterial* m_material;

		std::vector<LibMath::Vector3> m_debugPoints;

		bool m_usePvd = false;


	};
}
