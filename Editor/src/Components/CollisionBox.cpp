#include "Components/CollisionBox.h"
#include "PhysicSimulation.h"

#include "CollisionBox.h"

#include "Scripting/ScriptManager.h"

namespace NIX::Editor
{
	CollisionBox::CollisionBox(GameObject* gameObject)
		: CollisionShapeComponent(gameObject, Physic::PhysicSimulation::GetSingleton()->CreateCollisionBox({ 1.0f, 1.0f, 1.0f }))
	{
		GetCollisionBox()->SetObjectScale(m_gameObject->GetTransform()->GetWorldScale());
	}

	CollisionBox::CollisionBox(GameObject* gameObject, const LibMath::Vector3& scale) :
		CollisionShapeComponent(gameObject, Physic::PhysicSimulation::GetSingleton()->CreateCollisionBox(scale)) 
	{
		GetCollisionBox()->SetObjectScale(m_gameObject->GetTransform()->GetWorldScale());
	}

	CollisionBox::CollisionBox(GameObject* gameObject, const CollisionBox& other) :
		CollisionShapeComponent(gameObject, Physic::PhysicSimulation::GetSingleton()->CreateCollisionBox(other.GetCollisionBox()->GetScale()))
	{
		m_collisionShape->SetIsTrigger(other.m_collisionShape->GetIsTrigger());
		m_collisionShape->SetIsQueryTestEnable(other.m_collisionShape->GetIsQueryTestEnable());
		m_collisionShape->IsDebugDraw(other.m_collisionShape->IsDebugDraw());
		//m_collisionShape->SetIsContactTestEnable(other.m_collisionShape->GetIsContactTestEnable());

		SetIsActive(gameObject->IsActive());
	}

	void CollisionBox::Serialize(Core::SaveElement& parent)
	{
		Core::SaveElement collisionBoxElem(parent, "collision_box");

		collisionBoxElem.AddVec3Element("scale", GetCollisionBox()->GetScale());
		collisionBoxElem.AddBoolAttribute("is_trigger", m_collisionShape->GetIsTrigger());
		collisionBoxElem.AddBoolAttribute("is_query_test_enable", m_collisionShape->GetIsQueryTestEnable());
	}

	void CollisionBox::Deserialize(const Core::SaveElement& data)
	{
		Physic::CollisionBox* collisionBox = GetCollisionBox();

		collisionBox->SetScale(data.GetVector3Element("scale"));
		collisionBox->SetIsTrigger(data.GetBoolAttribute("is_trigger"));
		collisionBox->SetIsQueryTestEnable(data.GetBoolAttribute("is_query_test_enable"));
	}


	Physic::CollisionBox* CollisionBox::GetCollisionBox() const
	{
		return (Physic::CollisionBox*)m_collisionShape;
	}

	void CollisionBox::LuaInit()
	{
		ScriptManager::GetSingleton()->AddClass<CollisionBox, GameObject*>("CollisionBox");

		ScriptManager::GetSingleton()->AddClassProperty<CollisionBox>("CollisionBox", "IsActive", &CollisionBox::GetIsActive, &CollisionBox::SetIsActive);
		ScriptManager::GetSingleton()->AddClassData<CollisionBox>("CollisionBox", "GameObject", &CollisionBox::m_gameObject, false);
		ScriptManager::GetSingleton()->AddClassFunction<CollisionBox>("CollisionBox", "GetGameObject", &CollisionBox::GetParent);

		ScriptManager::GetSingleton()->AddClassProperty<CollisionBox>("CollisionBox", "IsQueryTestEnable", &CollisionBox::GetIsQueryTestEnable, &CollisionBox::SetIsQueryTestEnable);
		ScriptManager::GetSingleton()->AddClassProperty<CollisionBox>("CollisionBox", "IsDebugDraw", &CollisionBox::GetIsDebugDraw, &CollisionBox::SetIsDebugDraw);
		ScriptManager::GetSingleton()->AddClassProperty<CollisionBox>("CollisionBox", "IsTrigger", &CollisionBox::GetIsTrigger, &CollisionBox::SetIsTrigger);

		ScriptManager::GetSingleton()->AddClassProperty<CollisionBox>("CollisionBox", "Scale", &CollisionBox::GetScale, &CollisionBox::SetScale);
		ScriptManager::GetSingleton()->AddClassProperty<CollisionBox>("CollisionBox", "ScaleX", &CollisionBox::GetScaleX, &CollisionBox::SetScaleX);
		ScriptManager::GetSingleton()->AddClassProperty<CollisionBox>("CollisionBox", "ScaleY", &CollisionBox::GetScaleY, &CollisionBox::SetScaleY);
		ScriptManager::GetSingleton()->AddClassProperty<CollisionBox>("CollisionBox", "ScaleZ", &CollisionBox::GetScaleZ, &CollisionBox::SetScaleZ);

	}

	void CollisionBox::SetScaleX(float scaleX)
	{
		GetCollisionBox()->SetScaleX(scaleX);
	}

	void CollisionBox::SetScaleY(float scaleY)
	{
		GetCollisionBox()->SetScaleY(scaleY);
	}

	void CollisionBox::SetScaleZ(float scaleZ)
	{
		GetCollisionBox()->SetScaleZ(scaleZ);
	}

	void CollisionBox::SetScale(LibMath::Vector3& scale)
	{
		GetCollisionBox()->SetScale(scale);
	}

	float CollisionBox::GetScaleX() const
	{
		return GetCollisionBox()->GetScaleX();
	}

	float CollisionBox::GetScaleY() const
	{
		return GetCollisionBox()->GetScaleY();
	}

	float CollisionBox::GetScaleZ() const
	{
		return GetCollisionBox()->GetScaleZ();
	}

	LibMath::Vector3 CollisionBox::GetScale() const
	{
		return GetCollisionBox()->GetScale();
	}

}