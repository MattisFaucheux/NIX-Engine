#include "pch.h"

#include "PhysicSimulationFilterShader.h"

namespace NIX::Physic
{
    PxFilterFlags PhysicSimulationFilterShader::ContactReportFilterShader(const PxFilterObjectAttributes attributes0, const PxFilterData filterData0, const PxFilterObjectAttributes attributes1, const PxFilterData filterData1, PxPairFlags& pairFlags, const void* constantBlock, const PxU32 constantBlockSize)
    {
        PX_UNUSED(attributes0);
        PX_UNUSED(attributes1);
        PX_UNUSED(filterData0);
        PX_UNUSED(filterData1);
        PX_UNUSED(constantBlockSize);
        PX_UNUSED(constantBlock);

        pairFlags = PxPairFlag::eSOLVE_CONTACT | PxPairFlag::eDETECT_DISCRETE_CONTACT
            | PxPairFlag::eNOTIFY_TOUCH_FOUND 
            | PxPairFlag::eNOTIFY_TOUCH_LOST
            /*| PxPairFlag::eNOTIFY_TOUCH_PERSISTS*/
            | PxPairFlag::eNOTIFY_CONTACT_POINTS;
        return PxFilterFlag::eDEFAULT;
    }
}