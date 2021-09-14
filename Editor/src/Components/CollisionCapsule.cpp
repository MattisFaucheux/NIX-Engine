#include "Components/CollisionCapsule.h"
#include "PhysicSimulation.h"
#include "Scripting/ScriptManager.h"

namespace NIX::Editor
{
	CollisionCapsule::CollisionCapsule(GameObject* gameObject)
		: CollisionShapeComponent(gameObject, Physic::PhysicSimulation::GetSingleton()->CreateCollisionCapsule(1.0f, 1.0f))
	{}

	CollisionCapsule::CollisionCapsule(GameObject* gameObject, float radius, float height) :
		CollisionShapeComponent(gameObject, Physic::PhysicSimulation::GetSingleton()->CreateCollisionCapsule(radius, height)) {}

	CollisionCapsule::CollisionCapsule(GameObject* gameObject, const CollisionCapsule& other) :
		CollisionShapeComponent(gameObject, Physic::PhysicSimulation::GetSingleton()->CreateCollisionCapsule(other.GetCollisionCapsule()->GetRadius(), other.GetCollisionCapsule()->GetHeight()))
	{
		m_collisionShape->SetIsTrigger(other.m_collisionShape->GetIsTrigger());
		m_collisionShape->SetIsQueryTestEnable(other.m_collisionShape->GetIsQueryTestEnable());
		m_collisionShape->IsDebugDraw(other.m_collisionShape->IsDebugDraw());

		SetIsActive(gameObject->IsActive());
	}

	void CollisionCapsule::Serialize(Core::SaveElement& parent)
	{
		Core::SaveElement collisionCapsuleElem(parent, "collision_capsule");

		collisionCapsuleElem.AddFloatAttribute("height", GetCollisionCapsule()->GetHeight());
		collisionCapsuleElem.AddFloatAttribute("radius", GetCollisionCapsule()->GetRadius());
		collisionCapsuleElem.AddBoolAttribute("is_trigger", m_collisionShape->GetIsTrigger());
		collisionCapsuleElem.AddBoolAttribute("is_query_test_enable", m_collisionShape->GetIsQueryTestEnable());
	}

	void CollisionCapsule::Deserialize(const Core::SaveElement& data)
	{
		Physic::CollisionCapsule* collisionCapsule = GetCollisionCapsule();
		
		collisionCapsule->SetGeometry(data.GetFloatAttribute("radius"), data.GetFloatAttribute("height"));
		collisionCapsule->SetIsTrigger(data.GetBoolAttribute("is_trigger"));
		collisionCapsule->SetIsQueryTestEnable(data.GetBoolAttribute("is_query_test_enable"));
	}

	Physic::CollisionCapsule* CollisionCapsule::GetCollisionCapsule() const
	{
		return (Physic::CollisionCapsule*)m_collisionShape;
	}

	void CollisionCapsule::LuaInit()
	{
		ScriptManager::GetSingleton()->AddClass<CollisionCapsule, GameObject*>("CollisionCapsule");

		ScriptManager::GetSingleton()->AddClassProperty<CollisionCapsule>("CollisionCapsule", "IsActive", &CollisionCapsule::GetIsActive, &CollisionCapsule::SetIsActive);
		ScriptManager::GetSingleton()->AddClassData<CollisionCapsule>("CollisionCapsule", "GameObject", &CollisionCapsule::m_gameObject, false);
		ScriptManager::GetSingleton()->AddClassFunction<CollisionCapsule>("CollisionCapsule", "GetGameObject", &CollisionCapsule::GetParent);

		ScriptManager::GetSingleton()->AddClassProperty<CollisionCapsule>("CollisionCapsule", "IsQueryTestEnable", &CollisionCapsule::GetIsQueryTestEnable, &CollisionCapsule::SetIsQueryTestEnable);
		ScriptManager::GetSingleton()->AddClassProperty<CollisionCapsule>("CollisionCapsule", "IsDebugDraw", &CollisionCapsule::GetIsDebugDraw, &CollisionCapsule::SetIsDebugDraw);
		ScriptManager::GetSingleton()->AddClassProperty<CollisionCapsule>("CollisionCapsule", "IsTrigger", &CollisionCapsule::GetIsTrigger, &CollisionCapsule::SetIsTrigger);

		ScriptManager::GetSingleton()->AddClassProperty<CollisionCapsule>("CollisionCapsule", "Radius", &CollisionCapsule::GetRadius, &CollisionCapsule::SetRadius);
		ScriptManager::GetSingleton()->AddClassProperty<CollisionCapsule>("CollisionCapsule", "Height", &CollisionCapsule::GetHeight, &CollisionCapsule::SetHeight);
		ScriptManager::GetSingleton()->AddClassFunction<CollisionCapsule>("CollisionCapsule", "SetGeometry", &CollisionCapsule::SetGeometry);
	}

	void CollisionCapsule::SetRadius(float radius)
	{
		GetCollisionCapsule()->SetRadius(radius);
	}

	void CollisionCapsule::SetHeight(float height)
	{
		GetCollisionCapsule()->SetHeight(height);
	}

	void CollisionCapsule::SetGeometry(float radius, float height)
	{
		GetCollisionCapsule()->SetGeometry(radius, height);
	}


	float CollisionCapsule::GetRadius() const
	{
		return GetCollisionCapsule()->GetRadius();
	}

	float CollisionCapsule::GetHeight() const
	{
		return GetCollisionCapsule()->GetHeight();
	}

}