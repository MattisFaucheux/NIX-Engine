#include "Components/RigidBody.h"
#include "PhysicSimulation.h"
#include "Components/CollisionShapeComponent.h"

#include <algorithm>

#include "Log/Logger.h"

#include "Scripting/ScriptManager.h"
#include "Scripting/LuaForceMode.h"


namespace NIX::Editor
{
	RigidBody::RigidBody(GameObject* gameObject)
		: Component(gameObject)
	{
		if (gameObject == nullptr)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_TRACE, "Gameobject is null when trying to create a rigidbody");
			return;
		}

		CollisionShapeComponent* collisionShape = gameObject->GetComponent<CollisionShapeComponent>();
		if (collisionShape == nullptr)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_TRACE, "Collision shape is null when trying to create a rigidbody");
		}

		m_rigidBody = Physic::PhysicSimulation::GetSingleton()->CreateRigidBody({0.0f, 0.0f, 0.0f}, collisionShape->GetCollisionShape());
	}

	RigidBody::RigidBody(GameObject* gameObject, const LibMath::Vector3& position, Physic::CollisionShape* collisionShape) :
		Component(gameObject), m_rigidBody(Physic::PhysicSimulation::GetSingleton()->CreateRigidBody(position, collisionShape))
	{
	}

	RigidBody::RigidBody(GameObject* gameObject, const RigidBody& other) :
		Component(gameObject), m_rigidBody(Physic::PhysicSimulation::GetSingleton()->CreateRigidBody(gameObject->GetTransform()->GetWorldPosition(), gameObject->GetComponent<CollisionShapeComponent>()->GetCollisionShape()))
	{
		m_rigidBody->SetMass(other.m_rigidBody->GetMass());
		m_rigidBody->SetIsGravity(other.m_rigidBody->GetIsGravity());
		m_rigidBody->SetIsKinematic(other.m_rigidBody->GetIsKinematic());
		m_rigidBody->SetIsAxisRotationLock(other.m_rigidBody->GetIsXRotationLock(), other.m_rigidBody->GetIsYRotationLock(), other.m_rigidBody->GetIsZRotationLock());
		m_rigidBody->SetIsAxisTranslationLock(other.m_rigidBody->GetIsXTranslationLock(), other.m_rigidBody->GetIsYTranslationLock(), other.m_rigidBody->GetIsZTranslationLock());
		m_rigidBody->SetLinearDamping(other.m_rigidBody->GetLinearDamping());
		m_rigidBody->SetAngularDamping(other.m_rigidBody->GetAngularDamping());
		m_rigidBody->SetMaxLinearVelocity(other.m_rigidBody->GetMaxLinearVelocity());
		m_rigidBody->SetMaxAngularVelocity(other.m_rigidBody->GetMaxAngularVelocity());
		//m_rigidBody->SetMassInertiaTensor(other.m_rigidBody->GetMassInertiaTensor());

		SetIsActive(gameObject->IsActive());
	}

	RigidBody::~RigidBody()
	{
		delete m_rigidBody;
	}

	void RigidBody::Serialize(Core::SaveElement& parent)
	{
		Core::SaveElement rigidBodyElem(parent, "rigid_body");

		rigidBodyElem.AddBoolAttribute("is_gravity", m_rigidBody->GetIsGravity());
		rigidBodyElem.AddBoolAttribute("is_kinematic", m_rigidBody->GetIsKinematic());
		rigidBodyElem.AddBoolAttribute("is_active", m_rigidBody->IsActive());
		rigidBodyElem.AddFloatAttribute("angular_damping", m_rigidBody->GetAngularDamping());
		rigidBodyElem.AddFloatAttribute("linear_damping", m_rigidBody->GetLinearDamping());
		rigidBodyElem.AddFloatAttribute("max_angular_velocity", m_rigidBody->GetMaxAngularVelocity());
		rigidBodyElem.AddFloatAttribute("max_linear_velocity", m_rigidBody->GetMaxLinearVelocity());
		rigidBodyElem.AddFloatAttribute("mass", m_rigidBody->GetMass());
		
		rigidBodyElem.AddVec3Element("position", m_rigidBody->GetPosition());
		rigidBodyElem.AddVec4Element("rotation", m_rigidBody->GetRotation());		
		rigidBodyElem.AddVec3BoolElement("is_axis_rotation_lock", m_rigidBody->GetIsAxisRotationLock());
		rigidBodyElem.AddVec3BoolElement("is_axis_translation_lock", m_rigidBody->GetIsAxisTranslationLock());
		rigidBodyElem.AddVec3Element("mass_inertia_tensor", m_rigidBody->GetMassInertiaTensor());
	}

	void RigidBody::Deserialize(const Core::SaveElement& data)
	{
		Physic::RigidBody* rigidBody = GetRigidBody();

		rigidBody->SetIsGravity(data.GetBoolAttribute("is_gravity"));
		rigidBody->SetIsKinematic(data.GetBoolAttribute("is_kinematic"));
		rigidBody->IsActive(data.GetBoolAttribute("is_active"));
		rigidBody->SetAngularDamping(data.GetFloatAttribute("angular_damping"));
		rigidBody->SetLinearDamping(data.GetFloatAttribute("linear_damping"));
		rigidBody->SetMaxAngularVelocity(data.GetFloatAttribute("max_angular_velocity"));
		rigidBody->SetMaxLinearVelocity(data.GetFloatAttribute("max_linear_velocity"));
		rigidBody->SetMass(data.GetFloatAttribute("mass"));
		rigidBody->SetPosition(data.GetVector3Element("position"));
		rigidBody->SetRotation(data.GetQuaternionElement("rotation"));

		std::tuple<bool, bool, bool> isAxisRotaLock = data.GetVector3BoolElement("is_axis_rotation_lock");
		rigidBody->SetIsAxisRotationLock(std::get<0>(isAxisRotaLock), std::get<1>(isAxisRotaLock), std::get<2>(isAxisRotaLock));

		std::tuple<bool, bool, bool> isAxisTransLock = data.GetVector3BoolElement("is_axis_translation_lock");
		rigidBody->SetIsAxisTranslationLock(std::get<0>(isAxisTransLock), std::get<1>(isAxisTransLock), std::get<2>(isAxisTransLock));
		rigidBody->SetMassInertiaTensor(data.GetVector3Element("mass_inertia_tensor"));
	}

	void RigidBody::SetIsActive(bool value)
	{
		m_isActive = value;

		m_rigidBody->IsActive(value);
	}

	Physic::RigidBody* RigidBody::GetRigidBody() const
	{
		return m_rigidBody;
	}

	void RigidBody::LuaInit()
	{
		LuaForceMode::Init();

		ScriptManager::GetSingleton()->AddClass<RigidBody, GameObject*>("RigidBody");

		ScriptManager::GetSingleton()->AddClassProperty<RigidBody>("RigidBody", "IsActive", &RigidBody::GetIsActive, &RigidBody::SetIsActive);
		ScriptManager::GetSingleton()->AddClassData<RigidBody>("RigidBody", "GameObject", &RigidBody::m_gameObject, false);
		ScriptManager::GetSingleton()->AddClassFunction<RigidBody>("RigidBody", "GetGameObject", &RigidBody::GetParent);

		ScriptManager::GetSingleton()->AddClassProperty<RigidBody>("RigidBody", "Mass", &RigidBody::GetMass, &RigidBody::SetMass);
		ScriptManager::GetSingleton()->AddClassProperty<RigidBody>("RigidBody", "IsGravity", &RigidBody::GetIsGravity, &RigidBody::SetIsGravity);
		ScriptManager::GetSingleton()->AddClassProperty<RigidBody>("RigidBody", "IsKinematic", &RigidBody::GetIsKinematic, &RigidBody::SetIsKinematic);

		ScriptManager::GetSingleton()->AddClassProperty<RigidBody>("RigidBody", "IsXRotationLock", &RigidBody::GetIsXRotationLock, &RigidBody::SetIsXRotationLock);
		ScriptManager::GetSingleton()->AddClassProperty<RigidBody>("RigidBody", "IsYRotationLock", &RigidBody::GetIsYRotationLock, &RigidBody::SetIsYRotationLock);
		ScriptManager::GetSingleton()->AddClassProperty<RigidBody>("RigidBody", "IsZRotationLock", &RigidBody::GetIsZRotationLock, &RigidBody::SetIsZRotationLock);

		ScriptManager::GetSingleton()->AddClassProperty<RigidBody>("RigidBody", "IsXTranslationLock", &RigidBody::GetIsXTranslationLock, &RigidBody::SetIsXTranslationLock);
		ScriptManager::GetSingleton()->AddClassProperty<RigidBody>("RigidBody", "IsYTranslationLock", &RigidBody::GetIsYTranslationLock, &RigidBody::SetIsYTranslationLock);
		ScriptManager::GetSingleton()->AddClassProperty<RigidBody>("RigidBody", "IsZTranslationLock", &RigidBody::GetIsZTranslationLock, &RigidBody::SetIsZTranslationLock);

		ScriptManager::GetSingleton()->AddClassProperty<RigidBody>("RigidBody", "LinearDamping", &RigidBody::GetLinearDamping, &RigidBody::SetLinearDamping);
		ScriptManager::GetSingleton()->AddClassProperty<RigidBody>("RigidBody", "AngularDamping", &RigidBody::GetAngularDamping, &RigidBody::SetAngularDamping);
		ScriptManager::GetSingleton()->AddClassProperty<RigidBody>("RigidBody", "MaxLinearVelocity", &RigidBody::GetMaxLinearVelocity, &RigidBody::SetMaxLinearVelocity);
		ScriptManager::GetSingleton()->AddClassProperty<RigidBody>("RigidBody", "MaxAngularVelocity", &RigidBody::GetMaxAngularVelocity, &RigidBody::SetMaxAngularVelocity);
		ScriptManager::GetSingleton()->AddClassProperty<RigidBody>("RigidBody", "MassInertiaTensor", &RigidBody::GetMassInertiaTensor, &RigidBody::SetMassInertiaTensor);

		ScriptManager::GetSingleton()->AddClassFunction<RigidBody>("RigidBody", "GetAngularVelocity", &RigidBody::GetAngularVelocity);
		ScriptManager::GetSingleton()->AddClassFunction<RigidBody>("RigidBody", "GetLinearVelocity", &RigidBody::GetLinearVelocity);
		ScriptManager::GetSingleton()->AddClassFunction<RigidBody>("RigidBody", "GetLocalCenterOfMass", &RigidBody::GetLocalCenterOfMass);

		ScriptManager::GetSingleton()->AddClassFunction<RigidBody>("RigidBody", "ClearForce", &RigidBody::ClearForce);
		ScriptManager::GetSingleton()->AddClassFunction<RigidBody>("RigidBody", "ClearTorque", &RigidBody::ClearTorque);
		ScriptManager::GetSingleton()->AddClassFunction<RigidBody>("RigidBody", "ClearVelocity", &RigidBody::ClearVelocity);
		ScriptManager::GetSingleton()->AddClassFunction<RigidBody>("RigidBody", "GetIsSleeping", &RigidBody::GetIsSleeping);
		ScriptManager::GetSingleton()->AddClassFunction<RigidBody>("RigidBody", "WakeUp", &RigidBody::WakeUp);
		ScriptManager::GetSingleton()->AddClassFunction<RigidBody>("RigidBody", "PutToSleep", &RigidBody::PutToSleep);

		ScriptManager::GetSingleton()->AddClassFunction<RigidBody>("RigidBody", "AddForce", &RigidBody::AddForce);
		ScriptManager::GetSingleton()->AddClassFunction<RigidBody>("RigidBody", "AddForceAtPosition", &RigidBody::AddForceAtPosition);
		ScriptManager::GetSingleton()->AddClassFunction<RigidBody>("RigidBody", "AddLocalForceAtPosition", &RigidBody::AddLocalForceAtPosition);
		ScriptManager::GetSingleton()->AddClassFunction<RigidBody>("RigidBody", "AddForceAtLocalPosition", &RigidBody::AddForceAtLocalPosition);
		ScriptManager::GetSingleton()->AddClassFunction<RigidBody>("RigidBody", "AddLocalForceAtLocalPosition", &RigidBody::AddLocalForceAtLocalPosition);
		ScriptManager::GetSingleton()->AddClassFunction<RigidBody>("RigidBody", "AddTorque", &RigidBody::AddTorque);

		ScriptManager::GetSingleton()->AddClassFunction<RigidBody>("RigidBody", "SetPosition", &RigidBody::SetPosition);
		ScriptManager::GetSingleton()->AddClassFunction<RigidBody>("RigidBody", "SetRotation", &RigidBody::SetRotation);
	}

	void RigidBody::SetPosition(const LibMath::Vector3& position)
	{
		m_rigidBody->SetPosition(position);
	}

	void RigidBody::SetRotation(const LibMath::Quaternion& quaternion) 
	{
		m_rigidBody->SetRotation(quaternion);
	}

	float RigidBody::GetMass() const
	{
		return m_rigidBody->GetMass();
	}

	void RigidBody::SetMass(float mass)
	{
		m_rigidBody->SetMass(mass);
	}

	bool RigidBody::GetIsGravity() const
	{
		return m_rigidBody->GetIsGravity();
	}

	void RigidBody::SetIsGravity(bool isGravity)
	{
		m_rigidBody->SetIsGravity(isGravity);
	}

	bool RigidBody::GetIsKinematic() const
	{
		return m_rigidBody->GetIsKinematic();
	}

	void RigidBody::SetIsKinematic(bool isKinematic)
	{
		m_rigidBody->SetIsKinematic(isKinematic);
	}

	bool RigidBody::GetIsXRotationLock() const
	{
		return m_rigidBody->GetIsXRotationLock();
	}

	void RigidBody::SetIsXRotationLock(bool value)
	{
		m_rigidBody->SetIsXRotationLock(value);
	}

	bool RigidBody::GetIsYRotationLock() const
	{
		return m_rigidBody->GetIsYRotationLock();
	}

	void RigidBody::SetIsYRotationLock(bool value)
	{
		m_rigidBody->SetIsYRotationLock(value);
	}

	bool RigidBody::GetIsZRotationLock() const
	{
		return m_rigidBody->GetIsZRotationLock();
	}

	void RigidBody::SetIsZRotationLock(bool value)
	{
		m_rigidBody->SetIsZRotationLock(value);
	}

	bool RigidBody::GetIsXTranslationLock() const
	{
		return m_rigidBody->GetIsXTranslationLock();
	}

	void RigidBody::SetIsXTranslationLock(bool value)
	{
		m_rigidBody->SetIsXTranslationLock(value);
	}

	bool RigidBody::GetIsYTranslationLock() const
	{
		return m_rigidBody->GetIsYTranslationLock();
	}

	void RigidBody::SetIsYTranslationLock(bool value)
	{
		m_rigidBody->SetIsYTranslationLock(value);
	}

	bool RigidBody::GetIsZTranslationLock() const
	{
		return m_rigidBody->GetIsZTranslationLock();
	}

	void RigidBody::SetIsZTranslationLock(bool value)
	{
		m_rigidBody->SetIsZTranslationLock(value);
	}

	float RigidBody::GetLinearDamping() const
	{
		return m_rigidBody->GetLinearDamping();
	}

	void RigidBody::SetLinearDamping(float linearDamping)
	{
		m_rigidBody->SetLinearDamping(linearDamping);
	}

	float RigidBody::GetAngularDamping() const
	{
		return m_rigidBody->GetAngularDamping();
	}

	void RigidBody::SetAngularDamping(float angularDamping)
	{
		m_rigidBody->SetAngularDamping(angularDamping);
	}

	float RigidBody::GetMaxLinearVelocity() const
	{
		return m_rigidBody->GetMaxLinearVelocity();
	}

	void RigidBody::SetMaxLinearVelocity(float maxLinearVelocity)
	{
		m_rigidBody->SetMaxLinearVelocity(maxLinearVelocity);
	}

	float RigidBody::GetMaxAngularVelocity() const
	{
		return m_rigidBody->GetMaxAngularVelocity();
	}

	void RigidBody::SetMaxAngularVelocity(float maxAngularVelocity)
	{
		m_rigidBody->SetMaxAngularVelocity(maxAngularVelocity);
	}

	LibMath::Vector3 RigidBody::GetAngularVelocity() const
	{
		return m_rigidBody->GetAngularVelocity();
	}

	LibMath::Vector3 RigidBody::GetLinearVelocity() const
	{
		return m_rigidBody->GetLinearVelocity();
	}

	LibMath::Vector3 RigidBody::GetLocalCenterOfMass() const
	{
		return m_rigidBody->GetLocalCenterOfMass();
	}

	LibMath::Vector3 RigidBody::GetMassInertiaTensor() const
	{
		return m_rigidBody->GetMassInertiaTensor();
	}

	void RigidBody::SetMassInertiaTensor(const LibMath::Vector3& inertiaTensor)
	{
		m_rigidBody->SetMassInertiaTensor(inertiaTensor);
	}

	void RigidBody::ClearForce()
	{
		m_rigidBody->ClearForce();
	}

	void RigidBody::ClearTorque()
	{
		m_rigidBody->ClearTorque();
	}

	void RigidBody::ClearVelocity()
	{
		m_rigidBody->ClearVelocity();
	}

	void RigidBody::WakeUp()
	{
		m_rigidBody->WakeUp();
	}

	void RigidBody::PutToSleep()
	{
		m_rigidBody->PutToSleep();
	}

	bool RigidBody::GetIsSleeping()
	{
		return m_rigidBody->GetIsSleeping();
	}


	void RigidBody::AddForce(const LibMath::Vector3& force, std::string forceMode)
	{
		m_rigidBody->AddForce(force, LuaForceMode::GetForceMode(forceMode));
	}

	void RigidBody::AddForceAtPosition(const LibMath::Vector3& force, const LibMath::Vector3& pos, std::string forceMode)
	{
		m_rigidBody->AddForceAtPosition(force, pos, LuaForceMode::GetForceMode(forceMode));
	}

	void RigidBody::AddLocalForceAtPosition(const LibMath::Vector3& force, const LibMath::Vector3& pos, std::string forceMode)
	{
		m_rigidBody->AddLocalForceAtPosition(force, pos, LuaForceMode::GetForceMode(forceMode));
	}

	void RigidBody::AddForceAtLocalPosition(const LibMath::Vector3& force, const LibMath::Vector3& pos, std::string forceMode)
	{
		m_rigidBody->AddForceAtLocalPosition(force, pos, LuaForceMode::GetForceMode(forceMode));
	}

	void RigidBody::AddLocalForceAtLocalPosition(const LibMath::Vector3& force, const LibMath::Vector3& pos, std::string forceMode)
	{
		m_rigidBody->AddLocalForceAtLocalPosition(force, pos, LuaForceMode::GetForceMode(forceMode));
	}

	void RigidBody::AddTorque(const LibMath::Vector3& torque, std::string forceMode)
	{
		m_rigidBody->AddTorque(torque, LuaForceMode::GetForceMode(forceMode));
	}
}
