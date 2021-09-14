#pragma once
#include "DLLExport.h"

#include <PxPhysicsAPI.h>

namespace NIX::Physic
{
	using namespace physx;

	class PhysicSimulationFilterShader : public PxSimulationFilterCallback
	{
	public:

		static PxFilterFlags ContactReportFilterShader(const PxFilterObjectAttributes attributes0,
			const PxFilterData filterData0,
			const PxFilterObjectAttributes attributes1,
			const PxFilterData filterData1, PxPairFlags& pairFlags,
			const void* constantBlock, const PxU32 constantBlockSize);

	private:

	};
}
