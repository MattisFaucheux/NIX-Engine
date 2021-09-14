#include "Components/CollisionShapeComponent.h"

#include "PhysicSimulation.h"
#include "Components/RigidBody.h"
#include "Components/RigidStatic.h"

#include "Scripting/ScriptManager.h"

namespace NIX::Editor
{
	CollisionShapeComponent::CollisionShapeComponent(GameObject* gameObject, Physic::CollisionShape* collisionShape) :
		Component(gameObject), m_collisionShape(collisionShape)
	{
		m_collisionShape->BindCollisionCallback(m_gameObject, &GameObject::OnContact);
		m_collisionShape->BindTriggerCallback(m_gameObject, &GameObject::OnTrigger);
	}

	CollisionShapeComponent::CollisionShapeComponent(GameObject* gameObject) :
		Component(gameObject), m_collisionShape(Physic::PhysicSimulation::GetSingleton()->CreateCollisionBox({ 0.0f, 0.0f, 0.0f }))
	{
		m_collisionShape->BindCollisionCallback(m_gameObject, &GameObject::OnContact);
		m_collisionShape->BindTriggerCallback(m_gameObject, &GameObject::OnTrigger);
	}

	void CollisionShapeComponent::Serialize(Core::SaveElement& parent)
	{
		Core::SaveElement collisionShapeElem(parent, "collision_shape");

		collisionShapeElem.AddBoolAttribute("is_trigger", m_collisionShape->GetIsTrigger());
		collisionShapeElem.AddBoolAttribute("is_query_test_enable", m_collisionShape->GetIsQueryTestEnable());
	}

	void CollisionShapeComponent::Deserialize(const Core::SaveElement& /*data*/)
	{
		// Need to be declare but nothing to do here
		LOG(SOURCE_INFO, Core::EVerbosity::LOG_TRACE, "Collision Shape Component should not be deserialized");
	}

	void CollisionShapeComponent::SetIsActive(bool value)
	{
		m_isActive = value;
	}

	Physic::CollisionShape* CollisionShapeComponent::GetCollisionShape()
	{
		return m_collisionShape;
	}

	CollisionShapeComponent::~CollisionShapeComponent()
	{
		delete m_collisionShape;

		RigidBody* rigidBody = m_gameObject->GetComponent<RigidBody>();
		if (rigidBody != nullptr)
		{
			m_gameObject->RemoveComponent(rigidBody);
		}

		RigidStatic* rigidStatic = m_gameObject->GetComponent<RigidStatic>();
		if (rigidStatic != nullptr)
		{
			m_gameObject->RemoveComponent(rigidStatic);
		}
	}

	void CollisionShapeComponent::LuaInit()
	{
		ScriptManager::GetSingleton()->AddClass<CollisionShapeComponent, GameObject*>("CollisionShape");

		ScriptManager::GetSingleton()->AddClassProperty<CollisionShapeComponent>("CollisionShape", "IsActive", &CollisionShapeComponent::GetIsActive, &CollisionShapeComponent::SetIsActive);
		ScriptManager::GetSingleton()->AddClassData<CollisionShapeComponent>("CollisionShape", "GameObject", &CollisionShapeComponent::m_gameObject, false);
		ScriptManager::GetSingleton()->AddClassFunction<CollisionShapeComponent>("CollisionShape", "GetGameObject", &CollisionShapeComponent::GetParent);

		ScriptManager::GetSingleton()->AddClassProperty<CollisionShapeComponent>("CollisionShape", "IsQueryTestEnable", &CollisionShapeComponent::GetIsQueryTestEnable, &CollisionShapeComponent::SetIsQueryTestEnable);
		ScriptManager::GetSingleton()->AddClassProperty<CollisionShapeComponent>("CollisionShape", "IsDebugDraw", &CollisionShapeComponent::GetIsDebugDraw, &CollisionShapeComponent::SetIsDebugDraw);
		ScriptManager::GetSingleton()->AddClassProperty<CollisionShapeComponent>("CollisionShape", "IsTrigger", &CollisionShapeComponent::GetIsTrigger, &CollisionShapeComponent::SetIsTrigger);
	}
	
	void CollisionShapeComponent::SetIsQueryTestEnable(bool isEnable)
	{
		m_collisionShape->SetIsQueryTestEnable(isEnable);
	}

	bool CollisionShapeComponent::GetIsQueryTestEnable() const
	{
		return m_collisionShape->GetIsQueryTestEnable();
	}

	void CollisionShapeComponent::SetIsTrigger(bool isTrigger)
	{
		m_collisionShape->SetIsTrigger(isTrigger);
	}

	bool CollisionShapeComponent::GetIsTrigger() const
	{
		return m_collisionShape->GetIsTrigger();
	}

	void CollisionShapeComponent::SetIsDebugDraw(bool isDebugDraw)
	{
		m_collisionShape->IsDebugDraw(isDebugDraw);
	}

	bool CollisionShapeComponent::GetIsDebugDraw() const
	{
		return m_collisionShape->IsDebugDraw();
	}

	bool CollisionShapeComponent::IsSameCollisionShape(Physic::CollisionShape* other)
	{
		if (m_collisionShape)
		{
			if (m_collisionShape == other)
			{
				return true;
			}
		}

		return false;
	}
}
