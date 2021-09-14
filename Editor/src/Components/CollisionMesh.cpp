#include "Components\CollisionMesh.h"
#include "PhysicSimulation.h"
#include "Scripting/ScriptManager.h"

namespace NIX::Editor
{
	CollisionMesh::CollisionMesh(GameObject* gameObject)
		: CollisionShapeComponent(gameObject, Physic::PhysicSimulation::GetSingleton()->CreateCollisionMesh({ {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} }, { 1.0f, 1.0f, 1.0f }))
	{}

	CollisionMesh::CollisionMesh(GameObject* gameObject, const std::vector<LibMath::Vector3>& points, const LibMath::Vector3& scale) :
		CollisionShapeComponent(gameObject, Physic::PhysicSimulation::GetSingleton()->CreateCollisionMesh(points, scale)) {}

	CollisionMesh::CollisionMesh(GameObject* gameObject, const CollisionMesh& other) :
		CollisionShapeComponent(gameObject, Physic::PhysicSimulation::GetSingleton()->CreateCollisionMesh(other.GetCollisionMesh()->GetPoints(), other.GetCollisionMesh()->GetScale()))
	{
		m_collisionShape->SetIsTrigger(other.m_collisionShape->GetIsTrigger());
		m_collisionShape->SetIsQueryTestEnable(other.m_collisionShape->GetIsQueryTestEnable());
		m_collisionShape->IsDebugDraw(other.m_collisionShape->IsDebugDraw());

		SetIsActive(gameObject->IsActive());
	}

	void CollisionMesh::Serialize(Core::SaveElement& parent)
	{
		Core::SaveElement collisionMeshElem(parent, "collision_mesh");

		collisionMeshElem.AddVec3Element("scale", GetCollisionMesh()->GetScale());
		collisionMeshElem.AddBoolAttribute("is_trigger", m_collisionShape->GetIsTrigger());
		collisionMeshElem.AddBoolAttribute("is_query_test_enable", m_collisionShape->GetIsQueryTestEnable());
	}

	void CollisionMesh::Deserialize(const Core::SaveElement& data)
	{
		Physic::CollisionMesh* collisionMesh = GetCollisionMesh();

		collisionMesh->SetScale(data.GetVector3Element("scale"));
		collisionMesh->SetIsTrigger(data.GetBoolAttribute("is_trigger"));
		collisionMesh->SetIsQueryTestEnable(data.GetBoolAttribute("is_query_test_enable"));
	}

	void CollisionMesh::SetPoints(const std::vector<LibMath::Vector3>& points)
	{
		GetCollisionMesh()->SetPoints(points);
	}

	Physic::CollisionMesh* CollisionMesh::GetCollisionMesh() const
	{
		return dynamic_cast<Physic::CollisionMesh*>(m_collisionShape);
	}

	void CollisionMesh::LuaInit()
	{
		ScriptManager::GetSingleton()->AddClass<CollisionMesh, GameObject*>("CollisionMesh");

		ScriptManager::GetSingleton()->AddClassProperty<CollisionMesh>("CollisionMesh", "IsActive", &CollisionMesh::GetIsActive, &CollisionMesh::SetIsActive);
		ScriptManager::GetSingleton()->AddClassData<CollisionMesh>("CollisionMesh", "GameObject", &CollisionMesh::m_gameObject, false);
		ScriptManager::GetSingleton()->AddClassFunction<CollisionMesh>("CollisionMesh", "GetGameObject", &CollisionMesh::GetParent);

		ScriptManager::GetSingleton()->AddClassProperty<CollisionMesh>("CollisionMesh", "IsQueryTestEnable", &CollisionMesh::GetIsQueryTestEnable, &CollisionMesh::SetIsQueryTestEnable);
		ScriptManager::GetSingleton()->AddClassProperty<CollisionMesh>("CollisionMesh", "IsDebugDraw", &CollisionMesh::GetIsDebugDraw, &CollisionMesh::SetIsDebugDraw);
		ScriptManager::GetSingleton()->AddClassProperty<CollisionMesh>("CollisionMesh", "IsTrigger", &CollisionMesh::GetIsTrigger, &CollisionMesh::SetIsTrigger);
	}
}