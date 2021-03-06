#pragma once

#include <PxPhysicsAPI.h>

namespace NIX::Physic
{
	using namespace physx;

	class PhysicSimulationCallback : public PxSimulationEventCallback
	{
	public:

		void onSleep(PxActor** actors, PxU32 count) override;
		void onWake(PxActor** actors, PxU32 count) override;
		void onTrigger(PxTriggerPair* pairs, PxU32 count) override;
		void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override;

		void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override;
		void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) override;

	private:
	};
}
