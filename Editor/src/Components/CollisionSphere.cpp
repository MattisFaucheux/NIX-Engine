#include "Components/CollisionSphere.h"
#include "PhysicSimulation.h"
#include "Scripting/ScriptManager.h"

namespace NIX::Editor
{
	CollisionSphere::CollisionSphere(GameObject* gameObject)
		: CollisionShapeComponent(gameObject, Physic::PhysicSimulation::GetSingleton()->CreateCollisionSphere(1.0f))
	{}

	CollisionSphere::CollisionSphere(GameObject* gameObject, float radius) :
		CollisionShapeComponent(gameObject, Physic::PhysicSimulation::GetSingleton()->CreateCollisionSphere(radius)) {}

	CollisionSphere::CollisionSphere(GameObject* gameObject, const CollisionSphere& other) :
		CollisionShapeComponent(gameObject, Physic::PhysicSimulation::GetSingleton()->CreateCollisionSphere(other.GetCollisionSphere()->GetRadius()))
	{
		m_collisionShape->SetIsTrigger(other.m_collisionShape->GetIsTrigger());
		m_collisionShape->SetIsQueryTestEnable(other.m_collisionShape->GetIsQueryTestEnable());
		m_collisionShape->IsDebugDraw(other.m_collisionShape->IsDebugDraw());
		//m_collisionShape->SetIsContactTestEnable(other.m_collisionShape->GetIsContactTestEnable());

		SetIsActive(gameObject->IsActive());
	}

	void CollisionSphere::Serialize(Core::SaveElement& parent)
	{
		Core::SaveElement collisionSphereElem(parent, "collision_sphere");

		collisionSphereElem.AddFloatAttribute("radius", GetCollisionSphere()->GetRadius());
		collisionSphereElem.AddBoolAttribute("is_trigger", m_collisionShape->GetIsTrigger());
		collisionSphereElem.AddBoolAttribute("is_query_test_enable", m_collisionShape->GetIsQueryTestEnable());
	}

	void CollisionSphere::Deserialize(const  Core::SaveElement& data)
	{
		Physic::CollisionSphere* collisionSphere = GetCollisionSphere();

		collisionSphere->SetRadius(data.GetFloatAttribute("radius"));
		collisionSphere->SetIsTrigger(data.GetBoolAttribute("is_trigger"));
		collisionSphere->SetIsQueryTestEnable(data.GetBoolAttribute("is_query_test_enable"));
	}

	Physic::CollisionSphere* CollisionSphere::GetCollisionSphere() const
	{
		return (Physic::CollisionSphere*)m_collisionShape;
	}

	void CollisionSphere::LuaInit()
	{
		ScriptManager::GetSingleton()->AddClass<CollisionSphere, GameObject*>("CollisionSphere");

		ScriptManager::GetSingleton()->AddClassProperty<CollisionSphere>("CollisionSphere", "IsActive", &CollisionSphere::GetIsActive, &CollisionSphere::SetIsActive);
		ScriptManager::GetSingleton()->AddClassData<CollisionSphere>("CollisionSphere", "GameObject", &CollisionSphere::m_gameObject, false);
		ScriptManager::GetSingleton()->AddClassFunction<CollisionSphere>("CollisionSphere", "GetGameObject", &CollisionSphere::GetParent);

		ScriptManager::GetSingleton()->AddClassProperty<CollisionSphere>("CollisionSphere", "IsQueryTestEnable", &CollisionSphere::GetIsQueryTestEnable, &CollisionSphere::SetIsQueryTestEnable);
		ScriptManager::GetSingleton()->AddClassProperty<CollisionSphere>("CollisionSphere", "IsDebugDraw", &CollisionSphere::GetIsDebugDraw, &CollisionSphere::SetIsDebugDraw);
		ScriptManager::GetSingleton()->AddClassProperty<CollisionSphere>("CollisionSphere", "IsTrigger", &CollisionSphere::GetIsTrigger, &CollisionSphere::SetIsTrigger);

		ScriptManager::GetSingleton()->AddClassProperty<CollisionSphere>("CollisionBox", "Radius", &CollisionSphere::GetRadius, &CollisionSphere::SetRadius);
	}

	void CollisionSphere::SetRadius(float radius)
	{
		GetCollisionSphere()->SetRadius(radius);
	}

	float CollisionSphere::GetRadius() const
	{
		return GetCollisionSphere()->GetRadius();
	}

}