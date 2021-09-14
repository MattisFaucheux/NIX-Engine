#include "pch.h"

#include "PhysicScene.h"

#include <PhysicSimulation.h>
#include <PxSceneDesc.h>

#include "PhysicSimulationFilterShader.h"

namespace NIX::Physic
{
	PhysicScene::PhysicScene()
	{
		PxPhysics* physics = PhysicSimulation::GetSingleton()->GetPhysics();
		
		//Create PxSceneDesc
		PxSceneDesc sceneDesc(  physics->getTolerancesScale());
		sceneDesc.gravity = PxVec3(0.0f, -9.81f * 1.0f, 0.0f);
		sceneDesc.cpuDispatcher = PhysicSimulation::GetSingleton()->GetDispatcher();
		sceneDesc.filterShader = PhysicSimulationFilterShader::ContactReportFilterShader;
		sceneDesc.simulationEventCallback = &PhysicSimulation::GetSingleton()->GetSimulationCallback();

		//Create PxScene*
		m_scene = physics->createScene(sceneDesc);

		if (m_scene == nullptr)
		{
			//LOG()
			return;
		}

		EnablePhysicsVizualization();
	}

	void PhysicScene::EnablePhysicsVizualization()
	{
		m_scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
		m_scene->setVisualizationParameter(PxVisualizationParameter::eACTOR_AXES, 0.0f);
		m_scene->setVisualizationParameter(PxVisualizationParameter::eBODY_AXES, 0.0f);
		m_scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, true);
	}

	void PhysicScene::SetScene(physx::PxScene* scene)
	{
		m_scene = scene;
	}

	physx::PxScene* PhysicScene::GetScene() const
	{
		return m_scene;
	}
}
