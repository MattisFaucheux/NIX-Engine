#include "Components/RigidStatic.h"
#include "PhysicSimulation.h"
#include "Components/CollisionShapeComponent.h"
#include "Log/Logger.h"

#include "Scripting/ScriptManager.h"

namespace NIX::Editor
{
	RigidStatic::RigidStatic(GameObject* gameObject)
		: Component(gameObject)
	{
		if (gameObject == nullptr)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_TRACE, "Gameobject is null when trying to create a rigidstatic");
			return;
		}
		
		CollisionShapeComponent* collisionShape = gameObject->GetComponent<CollisionShapeComponent>();
		if (collisionShape == nullptr)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_TRACE, "Collision shape is null when trying to create a rigidstatic");
		}
		
		m_rigidStatic = Physic::PhysicSimulation::GetSingleton()->CreateRigidStatic({ 0.0f, 0.0f, 0.0f }, collisionShape->GetCollisionShape());
		
	}

	RigidStatic::RigidStatic(GameObject* gameObject, const LibMath::Vector3& position, Physic::CollisionShape* collisionShape) :
		Component(gameObject), m_rigidStatic(Physic::PhysicSimulation::GetSingleton()->CreateRigidStatic(position, collisionShape))
	{
	}

	RigidStatic::RigidStatic(GameObject* gameObject, const RigidStatic& /*other*/) :
		Component(gameObject), m_rigidStatic(Physic::PhysicSimulation::GetSingleton()->CreateRigidStatic(gameObject->GetTransform()->GetWorldPosition(), gameObject->GetComponent<CollisionShapeComponent>()->GetCollisionShape()))
	{
		SetIsActive(gameObject->IsActive());
	}

	RigidStatic::~RigidStatic()
	{
		delete m_rigidStatic;
	}
	
	void RigidStatic::Serialize(Core::SaveElement& parent)
	{
		Core::SaveElement rigidStaticElem(parent, "rigid_static");

		rigidStaticElem.AddBoolAttribute("is_active", m_rigidStatic->IsActive());
		
		rigidStaticElem.AddVec3Element("position", m_rigidStatic->GetPosition());
		rigidStaticElem.AddVec4Element("rotation", m_rigidStatic->GetRotation());
	}

	void RigidStatic::Deserialize(const Core::SaveElement& data)
	{
		m_rigidStatic->IsActive(data.GetBoolAttribute("is_active"));
		m_rigidStatic->SetPosition(data.GetVector3Element("position"));
		m_rigidStatic->SetRotation(data.GetQuaternionElement("rotation"));
	}

	void RigidStatic::SetIsActive(bool value)
	{
		m_isActive = value;

		m_rigidStatic->IsActive(value);
	}

	Physic::RigidStatic* RigidStatic::GetRigidStatic()
	{
		return m_rigidStatic;
	}

	void RigidStatic::SetPosition(const LibMath::Vector3& position)
	{
		m_rigidStatic->SetPosition(position);
	}

	void RigidStatic::SetRotation(const LibMath::Quaternion& quaternion)
	{
		m_rigidStatic->SetRotation(quaternion);
	}

	void RigidStatic::LuaInit()
	{
		ScriptManager::GetSingleton()->AddClass<RigidStatic, GameObject*>("RigidStatic");

		ScriptManager::GetSingleton()->AddClassProperty<RigidStatic>("RigidStatic", "IsActive", &RigidStatic::GetIsActive, &RigidStatic::SetIsActive);
		ScriptManager::GetSingleton()->AddClassData<RigidStatic>("RigidStatic", "GameObject", &RigidStatic::m_gameObject, false);
		ScriptManager::GetSingleton()->AddClassFunction<RigidStatic>("RigidStatic", "GetGameObject", &RigidStatic::GetParent);

		ScriptManager::GetSingleton()->AddClassFunction<RigidStatic>("RigidStatic", "SetPosition", &RigidStatic::SetPosition);
		ScriptManager::GetSingleton()->AddClassFunction<RigidStatic>("RigidStatic", "SetRotation", &RigidStatic::SetRotation);
	}
}
