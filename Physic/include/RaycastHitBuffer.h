#pragma once
#include "DLLExport.h"

#include <PxPhysicsAPI.h>

#include "RaycastHit.h"


namespace NIX::Physic
{
	using namespace physx;

	struct RaycastHitBuffer
	{
		PHYSIC_LIBRARY_API [[nodiscard]] bool HasBlock() const;
		PHYSIC_LIBRARY_API [[nodiscard]] bool HasAnyHit();
		PHYSIC_LIBRARY_API [[nodiscard]] int GetNbAnyHits() const;
		PHYSIC_LIBRARY_API [[nodiscard]] int GetNbTouches() const;

		PHYSIC_LIBRARY_API [[nodiscard]] const RaycastHit& GetBlockInfo() const;
		PHYSIC_LIBRARY_API [[nodiscard]] RaycastHit* GetTouchesInfo() const;
		PHYSIC_LIBRARY_API [[nodiscard]] RaycastHit GetTouchInfo(unsigned int index) const;

		[[nodiscard]] PxRaycastBuffer& GetOriginalBuffer();
		void UpdateRaycastHitInfo();

	private:
		
		RaycastHit* touches = nullptr;
		RaycastHit block;
		PxRaycastBuffer originalBuffer;
	};
}
