#pragma once
#include "DLLExport.h"

#include <PxPhysicsAPI.h>

#include <functional>

namespace NIX::Physic
{
	using namespace physx;

	class CollisionShape
	{
	public:

		CollisionShape() = delete;
		PHYSIC_LIBRARY_API CollisionShape(PxPhysics* physic, const PxMaterial& physicMat, const PxBoxGeometry& shapeGeometry);
		PHYSIC_LIBRARY_API CollisionShape(PxPhysics* physic, const PxMaterial& physicMat, const PxCapsuleGeometry& shapeGeometry);
		PHYSIC_LIBRARY_API CollisionShape(PxPhysics* physic, const PxMaterial& physicMat, const PxConvexMeshGeometry& shapeGeometry);
		PHYSIC_LIBRARY_API CollisionShape(PxPhysics* physic, const PxMaterial& physicMat, const PxSphereGeometry& shapeGeometry);
		CollisionShape(const CollisionShape& other) = delete;
		CollisionShape(CollisionShape&& other) noexcept = delete;
		CollisionShape& operator=(const CollisionShape& other) = delete;
		CollisionShape& operator=(CollisionShape&& other) noexcept = delete;
		PHYSIC_LIBRARY_API virtual ~CollisionShape();

		//Enable/Disable Collisions
		//PHYSIC_LIBRARY_API void SetIsContactTestEnable(bool isEnable);
		//PHYSIC_LIBRARY_API [[nodiscard]]  bool GetIsContactTestEnable();

		//Enable/Disable Collision with raycast
		PHYSIC_LIBRARY_API void SetIsQueryTestEnable(bool isEnable);
		PHYSIC_LIBRARY_API [[nodiscard]] bool GetIsQueryTestEnable() const;

		//Enable/Disable Trigger
		PHYSIC_LIBRARY_API void SetIsTrigger(bool isTrigger);
		PHYSIC_LIBRARY_API [[nodiscard]] bool GetIsTrigger() const;

		//Enable/Disable Debug Draw
		PHYSIC_LIBRARY_API void IsDebugDraw(bool isDebugDraw);
		PHYSIC_LIBRARY_API [[nodiscard]] bool IsDebugDraw() const;

		template<class T>
		PHYSIC_LIBRARY_API void BindCollisionCallback(T* t, void(T::* method)(CollisionShape*, bool))
		{
			m_callbackCollisionFunction = std::bind(method, t, std::placeholders::_1, std::placeholders::_2);
		}

		template<class T>
		PHYSIC_LIBRARY_API void BindTriggerCallback(T* t, void(T::* method)(CollisionShape*, bool))
		{
			m_callbackTriggerFunction = std::bind(method, t, std::placeholders::_1, std::placeholders::_2);
		}


		[[nodiscard]] PxShape* GetShape();

		void OnCollision(CollisionShape* otherShape, bool collisionBegin);
		void OnTrigger(CollisionShape* otherShape, bool triggerEnter);

	protected:

		std::function<void(CollisionShape*, bool)> m_callbackCollisionFunction;
		std::function<void(CollisionShape*, bool)> m_callbackTriggerFunction;

		PxShape* m_shape;
	};
}