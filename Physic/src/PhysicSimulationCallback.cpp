#include "pch.h"

#include "PhysicSimulationCallback.h"
#include "CollisionShape.h"

namespace NIX::Physic
{
	void PhysicSimulationCallback::onSleep(PxActor** /*actors*/ , PxU32 /*count*/ )
	{
		/**/
	}

	void PhysicSimulationCallback::onWake(PxActor** /*actors*/, PxU32 /*count*/)
	{
		/**/
	}

	void PhysicSimulationCallback::onTrigger(PxTriggerPair* pairs, PxU32 count)
	{
		for (PxU32 pairIndex = 0; pairIndex < count; pairIndex++)
		{
			const PxTriggerPair& triggerPair = pairs[pairIndex];

			PxShape* otherShape = reinterpret_cast<PxShape*>(triggerPair.otherShape);
			CollisionShape* otherCs = reinterpret_cast<CollisionShape*>(otherShape->userData);

			PxShape* triggerShape = reinterpret_cast<PxShape*>(triggerPair.triggerShape);
			CollisionShape* triggerCs = reinterpret_cast<CollisionShape*>(triggerShape->userData);

			if (triggerPair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				triggerCs->OnTrigger(otherCs, true);
			}
			else if (triggerPair.status == PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				triggerCs->OnTrigger(otherCs, false);
			}

		}
	}

	void PhysicSimulationCallback::onContact(const PxContactPairHeader& /*pairHeader*/, const PxContactPair* pairs, PxU32 nbPairs)
	{
		for (PxU32 pairIndex = 0; pairIndex < nbPairs; pairIndex++)
		{
			const PxContactPair& contactPair = pairs[pairIndex];

			if (contactPair.events.isSet(PxPairFlag::eNOTIFY_TOUCH_FOUND) &&
				!contactPair.flags.isSet(PxContactPairFlag::eACTOR_PAIR_LOST_TOUCH))
			{
				PxShape* firstShape = reinterpret_cast<PxShape*>(pairs->shapes[0]);
				CollisionShape* firstCs = reinterpret_cast<CollisionShape*>(firstShape->userData);

				PxShape* secondShape = reinterpret_cast<PxShape*>(pairs->shapes[1]);
				CollisionShape* secondCs = reinterpret_cast<CollisionShape*>(secondShape->userData);

				firstCs->OnCollision(secondCs, true);
				secondCs->OnCollision(firstCs, true);
			}
			else if (contactPair.events.isSet(PxPairFlag::eNOTIFY_TOUCH_LOST))
			{
				PxShape* firstShape = reinterpret_cast<PxShape*>(pairs->shapes[0]);
				CollisionShape* firstCs = reinterpret_cast<CollisionShape*>(firstShape->userData);

				PxShape* secondShape = reinterpret_cast<PxShape*>(pairs->shapes[1]);
				CollisionShape* secondCs = reinterpret_cast<CollisionShape*>(secondShape->userData);

				firstCs->OnCollision(secondCs, false);
				secondCs->OnCollision(firstCs, false);
			}

		}
	}

	void PhysicSimulationCallback::onConstraintBreak(PxConstraintInfo* /*constraints*/, PxU32 /*count*/)
	{
		/**/
	}

	void PhysicSimulationCallback::onAdvance(const PxRigidBody* const* /*bodyBuffer*/, const PxTransform* /*poseBuffer*/, const PxU32 /*count*/)
	{
		/**/
	}
}
