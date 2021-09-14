#pragma once

#include "DLLExport.h"

namespace physx {
	class PxScene;
}

namespace NIX::Physic
{
	class PhysicScene
	{
	private:
		physx::PxScene* m_scene;

	public:

		PHYSIC_LIBRARY_API PhysicScene();

	
		
		void SetScene(physx::PxScene* scene);
		[[nodiscard]] physx::PxScene* GetScene() const;

	private:
		void EnablePhysicsVizualization();
	};
}
