#include "Transform.h"

#include "Matrix/Matrix3.h"

#include "GameObject.h"

#include "Scripting/ScriptManager.h"

//////////////////////////////////////////////////////////////////////////////////

NIX::Editor::Transform::Transform(GameObject* gameObject, LibMath::Vector3 localPos, LibMath::Quaternion localRot, LibMath::Vector3 localScale) :
	Component(gameObject)
{
	localRot.Normalize();

	m_localMatrix = LibMath::Matrix4::Translation(localPos) * LibMath::Matrix4::Rotation(localRot) * LibMath::Matrix4::Scaling(localScale);
	m_localPosition = localPos;
	m_localRotation = localRot;
	m_localScale = localScale;

	UpdateWorldMatrix();
}

NIX::Editor::Transform::Transform(GameObject* gameObject, const Transform& other) :
	Component(gameObject)
{
	LibMath::Quaternion rot = other.m_localRotation;
	rot.Normalize();

	m_localMatrix = LibMath::Matrix4::Translation(other.m_localPosition) * LibMath::Matrix4::Rotation(rot) * LibMath::Matrix4::Scaling(other.m_localScale);
	m_localPosition = other.m_localPosition;
	m_localRotation = rot;
	m_localScale = other.m_localScale;

	UpdateWorldMatrix();

	SetIsActive(gameObject->IsActive());
}

NIX::Editor::Transform NIX::Editor::Transform::operator-(Transform trans)
{
	return Transform(m_gameObject, m_localPosition - trans.GetWorldPosition(), m_localRotation * trans.GetWorldRotation(), { m_localScale / trans.GetWorldScale()});
}

void NIX::Editor::Transform::Serialize(Core::SaveElement& parent)
{
	Core::SaveElement transform(parent, "transform");

	transform.AddVec3Element("position", m_localPosition);
	transform.AddVec4Element("rotation", m_localRotation);
	transform.AddVec3Element("scale", m_localScale);
}

void NIX::Editor::Transform::Deserialize(const Core::SaveElement& data)
{
	SetLocalPosition(data.GetVector3Element("position"));
	SetLocalRotation(data.GetQuaternionElement("rotation"));
	SetLocalScale(data.GetVector3Element("scale"));
}

void NIX::Editor::Transform::SetIsActive(bool value)
{
	m_isActive = value;
}

void NIX::Editor::Transform::SetLocal(LibMath::Matrix4 transform)
{
	m_localMatrix = transform;

	m_localPosition.x = transform[3][0];
	m_localPosition.y = transform[3][1];
	m_localPosition.z = transform[3][2];

	LibMath::Vector3 columns[3] =
	{
		{ transform[0][0], transform[0][1], transform[0][2]},
		{ transform[1][0], transform[1][1], transform[1][2]},
		{ transform[2][0], transform[2][1], transform[2][2]},
	};

	m_localScale.x = LibMath::Vector3::Magnitude(columns[0]);
	m_localScale.y = LibMath::Vector3::Magnitude(columns[1]);
	m_localScale.z = LibMath::Vector3::Magnitude(columns[2]);

	if (m_localScale.x)
	{
		columns[0] /= m_localScale.x;
	}
	if (m_localScale.y)
	{
		columns[1] /= m_localScale.y;
	}
	if (m_localScale.z)
	{
		columns[2] /= m_localScale.z;
	}

	LibMath::Matrix3 rotMat
	(
		columns[0].x, columns[1].x, columns[2].x,
		columns[0].y, columns[1].y, columns[2].y,
		columns[0].z, columns[1].z, columns[2].z
	);

	LibMath::Quaternion quat;

	float tr = rotMat[0][0] + rotMat[1][1] + rotMat[2][2];

	if (tr > 0.f)
	{
		float S = LibMath::sqrt(tr + 1.f) * 2.f; // S=4*qw 
		quat.W = 0.25f * S;
		quat.X = (rotMat[2][1] - rotMat[1][2]) / S;
		quat.Y = (rotMat[0][2] - rotMat[2][0]) / S;
		quat.Z = (rotMat[1][0] - rotMat[0][1]) / S;
	}
	else if ((rotMat[0][0] > rotMat[1][1]) && (rotMat[0][0] > rotMat[2][2]))
	{
		float S = LibMath::sqrt(1.f + rotMat[0][0] - rotMat[1][1] - rotMat[2][2]) * 2.f; // S=4*qx 
		quat.W = (rotMat[2][1] - rotMat[1][2]) / S;
		quat.X = 0.25f * S;
		quat.Y = (rotMat[0][1] + rotMat[1][0]) / S;
		quat.Z = (rotMat[0][2] + rotMat[2][0]) / S;
	}
	else if (rotMat[1][1] > rotMat[2][2])
	{
		float S = LibMath::sqrt(1.f + rotMat[1][1] - rotMat[0][0] - rotMat[2][2]) * 2.f; // S=4*qy
		quat.W = (rotMat[0][2] - rotMat[2][0]) / S;
		quat.X = (rotMat[0][1] + rotMat[1][0]) / S;
		quat.Y = 0.25f * S;
		quat.Z = (rotMat[1][2] + rotMat[2][1]) / S;
	}
	else
	{
		float S = LibMath::sqrt(1.f + rotMat[2][2] - rotMat[0][0] - rotMat[1][1]) * 2.f; // S=4*qz
		quat.W = (rotMat[1][0] - rotMat[0][1]) / S;
		quat.X = (rotMat[0][2] + rotMat[2][0]) / S;
		quat.Y = (rotMat[1][2] + rotMat[2][1]) / S;
		quat.Z = 0.25f * S;
	}

	m_localRotation = quat;

	UpdateWorldMatrix();
}

void NIX::Editor::Transform::SetLocal(LibMath::Vector3 pos, LibMath::Quaternion rot, LibMath::Vector3 scale)
{
	rot.Normalize();

	m_localMatrix = LibMath::Matrix4::Translation(pos) * LibMath::Matrix4::Rotation(rot) * LibMath::Matrix4::Scaling(scale);
	m_localPosition = pos;
	m_localRotation = rot;
	m_localScale = scale;

	UpdateWorldMatrix();
}

void NIX::Editor::Transform::SetWorld(LibMath::Matrix4 transform)
{
	if(m_gameObject->GetParent())
		SetLocal(m_gameObject->GetParent()->GetTransform()->GetWorldMatrix().GetInverse() * transform);
	else
		SetLocal(transform);
}

void NIX::Editor::Transform::SetWorld(LibMath::Vector3 pos, LibMath::Quaternion rot, LibMath::Vector3 scale)
{
	rot.Normalize();
	if (m_gameObject->GetParent())
		SetLocal(m_gameObject->GetParent()->GetTransform()->GetWorldMatrix().GetInverse() * LibMath::Matrix4::Translation(pos) * LibMath::Matrix4::Rotation(rot) * LibMath::Matrix4::Scaling(scale));
	else
		SetLocal(LibMath::Matrix4::Translation(pos) * LibMath::Matrix4::Rotation(rot) * LibMath::Matrix4::Scaling(scale));
}

void NIX::Editor::Transform::UpdateWorldMatrix()
{
	m_worldMatrix = m_gameObject->GetParent() ? m_gameObject->GetParent()->GetTransform()->m_worldMatrix * m_localMatrix : m_localMatrix;

	m_gameObject->Update();

	for (GameObject* child : m_gameObject->GetChildren())
	{
		child->GetTransform()->UpdateWorldMatrix();
	}
}

void NIX::Editor::Transform::SetPosition(LibMath::Vector3 pos)
{
	SetWorld(pos, m_localRotation, m_localScale);
}

void NIX::Editor::Transform::SetRotation(LibMath::Quaternion rot)
{
	SetWorld(m_localPosition, rot, m_localScale);
}

void NIX::Editor::Transform::SetScale(LibMath::Vector3 scale)
{
	SetWorld(m_localPosition, m_localRotation, scale);
}

void NIX::Editor::Transform::SetLocalPosition(LibMath::Vector3 pos)
{
	SetLocal(pos, m_localRotation, m_localScale);
}

void NIX::Editor::Transform::SetLocalRotation(LibMath::Quaternion rot)
{
	SetLocal(m_localPosition, rot, m_localScale);
}

void NIX::Editor::Transform::SetLocalScale(LibMath::Vector3 scale)
{
	SetLocal(m_localPosition, m_localRotation, scale);
}

void NIX::Editor::Transform::TranslateLocal(const LibMath::Vector3& translation)
{
	SetLocalPosition(m_localPosition + translation);
}

void NIX::Editor::Transform::RotateLocal(const LibMath::Quaternion& rotation)
{
	SetLocalRotation(m_localRotation * rotation);
}

void NIX::Editor::Transform::ScaleLocal(const LibMath::Vector3& scale)
{
	SetLocalScale(LibMath::Vector3(m_localScale.x * scale.x, m_localScale.y * scale.y, m_localScale.z * scale.z));
}

const LibMath::Vector3& NIX::Editor::Transform::GetLocalPosition() const
{
	return m_localPosition;
}

const LibMath::Quaternion& NIX::Editor::Transform::GetLocalRotation() const
{
	return m_localRotation;
}

const LibMath::Vector3& NIX::Editor::Transform::GetLocalScale() const
{
	return m_localScale;
}

LibMath::Vector3 NIX::Editor::Transform::GetWorldPosition() const
{
	return { m_worldMatrix[3][0], m_worldMatrix[3][1] , m_worldMatrix[3][2] };
}

LibMath::Quaternion NIX::Editor::Transform::GetWorldRotation() const
{
	LibMath::Vector3 columns[3] =
	{
		{ m_worldMatrix[0][0], m_worldMatrix[0][1], m_worldMatrix[0][2]},
		{ m_worldMatrix[1][0], m_worldMatrix[1][1], m_worldMatrix[1][2]},
		{ m_worldMatrix[2][0], m_worldMatrix[2][1], m_worldMatrix[2][2]}
	};

	LibMath::Vector3 worldScale = GetWorldScale();

	if (worldScale.x)
		columns[0] /= worldScale.x;
	if (worldScale.y)
		columns[1] /= worldScale.y;
	if (worldScale.z)
		columns[2] /= worldScale.z;

	LibMath::Matrix3 rotMat
	(
		columns[0].x, columns[1].x, columns[2].x,
		columns[0].y, columns[1].y, columns[2].y,
		columns[0].z, columns[1].z, columns[2].z
	);

	LibMath::Quaternion quat;

	float tr = rotMat[0][0] + rotMat[1][1] + rotMat[2][2];

	if (tr > 0.f)
	{
		float S = LibMath::sqrt(tr + 1.f) * 2.f; // S=4*qw 
		quat.W = 0.25f * S;
		quat.X = (rotMat[2][1] - rotMat[1][2]) / S;
		quat.Y = (rotMat[0][2] - rotMat[2][0]) / S;
		quat.Z = (rotMat[1][0] - rotMat[0][1]) / S;
	}
	else if ((rotMat[0][0] > rotMat[1][1]) && (rotMat[0][0] > rotMat[2][2]))
	{
		float S = LibMath::sqrt(1.f + rotMat[0][0] - rotMat[1][1] - rotMat[2][2]) * 2.f; // S=4*qx 
		quat.W = (rotMat[2][1] - rotMat[1][2]) / S;
		quat.X = 0.25f * S;
		quat.Y = (rotMat[0][1] + rotMat[1][0]) / S;
		quat.Z = (rotMat[0][2] + rotMat[2][0]) / S;
	}
	else if (rotMat[1][1] > rotMat[2][2])
	{
		float S = LibMath::sqrt(1.f + rotMat[1][1] - rotMat[0][0] - rotMat[2][2]) * 2.f; // S=4*qy
		quat.W = (rotMat[0][2] - rotMat[2][0]) / S;
		quat.X = (rotMat[0][1] + rotMat[1][0]) / S;
		quat.Y = 0.25f * S;
		quat.Z = (rotMat[1][2] + rotMat[2][1]) / S;
	}
	else
	{
		float S = LibMath::sqrt(1.f + rotMat[2][2] - rotMat[0][0] - rotMat[1][1]) * 2.f; // S=4*qz
		quat.W = (rotMat[1][0] - rotMat[0][1]) / S;
		quat.X = (rotMat[0][2] + rotMat[2][0]) / S;
		quat.Y = (rotMat[1][2] + rotMat[2][1]) / S;
		quat.Z = 0.25f * S;
	}

	return quat;
}

LibMath::Vector3 NIX::Editor::Transform::GetWorldScale() const
{
	return {LibMath::Vector3::Magnitude({ m_worldMatrix[0][0], m_worldMatrix[0][1], m_worldMatrix[0][2] }),
		LibMath::Vector3::Magnitude({ m_worldMatrix[1][0], m_worldMatrix[1][1], m_worldMatrix[1][2] }),
		LibMath::Vector3::Magnitude({ m_worldMatrix[2][0], m_worldMatrix[2][1], m_worldMatrix[2][2] })};
}

const LibMath::Matrix4& NIX::Editor::Transform::GetLocalMatrix() const
{
	return m_localMatrix;
}

const LibMath::Matrix4& NIX::Editor::Transform::GetWorldMatrix() const
{
	return m_worldMatrix;
}

LibMath::Vector3 NIX::Editor::Transform::GetWorldForward() const
{
	return GetWorldRotation() * LibMath::Vector3::Front;
}

LibMath::Vector3 NIX::Editor::Transform::GetWorldUp() const
{
	return GetWorldRotation() * LibMath::Vector3::Up;
}

LibMath::Vector3 NIX::Editor::Transform::GetWorldRight() const
{
	return GetWorldRotation() * LibMath::Vector3::Right;
}

LibMath::Vector3 NIX::Editor::Transform::GetLocalForward() const
{
	return m_localRotation * LibMath::Vector3::Front;
}

LibMath::Vector3 NIX::Editor::Transform::GetLocalUp() const
{
	return m_localRotation * LibMath::Vector3::Up;
}

LibMath::Vector3 NIX::Editor::Transform::GetLocalRight() const
{
	return m_localRotation * LibMath::Vector3::Right;
}

void NIX::Editor::Transform::LuaInit()
{
	ScriptManager::GetSingleton()->AddClass<Transform, GameObject*, LibMath::Vector3, LibMath::Quaternion, LibMath::Vector3>("Transform");

	ScriptManager::GetSingleton()->AddClassProperty<Transform>("Transform", "IsActive", &Transform::GetIsActive, &Transform::SetIsActive);
	ScriptManager::GetSingleton()->AddClassData<Transform>("Transform", "GameObject", &Transform::m_gameObject, false);
	ScriptManager::GetSingleton()->AddClassFunction<Transform>("Transform", "GetGameObject", &Transform::GetParent);

	ScriptManager::GetSingleton()->AddClassProperty<Transform>("Transform", "LocalPosition", &Transform::GetLocalPositionLua, &Transform::SetLocalPosition);
	ScriptManager::GetSingleton()->AddClassProperty<Transform>("Transform", "LocalRotation", &Transform::GetLocalRotationLua, &Transform::SetLocalRotation);
	ScriptManager::GetSingleton()->AddClassProperty<Transform>("Transform", "LocalScale", &Transform::GetLocalScaleLua, &Transform::SetLocalScale);

	ScriptManager::GetSingleton()->AddClassProperty<Transform>("Transform", "WorldPosition", &Transform::GetWorldPosition, &Transform::SetPosition);
	ScriptManager::GetSingleton()->AddClassProperty<Transform>("Transform", "WorldRotation", &Transform::GetWorldRotation, &Transform::SetRotation);
	ScriptManager::GetSingleton()->AddClassProperty<Transform>("Transform", "WorldScale", &Transform::GetWorldScale, &Transform::SetScale);

	ScriptManager::GetSingleton()->AddClassFunction<Transform>("Transform", "GetWorldForward", &Transform::GetWorldForward);
	ScriptManager::GetSingleton()->AddClassFunction<Transform>("Transform", "GetWorldUp", &Transform::GetWorldUp);
	ScriptManager::GetSingleton()->AddClassFunction<Transform>("Transform", "GetWorldRight", &Transform::GetWorldRight);

	ScriptManager::GetSingleton()->AddClassFunction<Transform>("Transform", "GetLocalForward", &Transform::GetLocalForward);
	ScriptManager::GetSingleton()->AddClassFunction<Transform>("Transform", "GetLocalUp", &Transform::GetLocalUp);
	ScriptManager::GetSingleton()->AddClassFunction<Transform>("Transform", "GetLocalRight", &Transform::GetLocalRight);

	ScriptManager::GetSingleton()->AddClassFunction<Transform>("Transform", "SetLocal", &Transform::SetLocalLua);
	ScriptManager::GetSingleton()->AddClassFunction<Transform>("Transform", "SetWorld", &Transform::SetWorldLua);

	ScriptManager::GetSingleton()->AddClassFunction<Transform>("Transform", "TranslateLocal", &Transform::TranslateLocal);
	ScriptManager::GetSingleton()->AddClassFunction<Transform>("Transform", "RotateLocal", &Transform::RotateLocal);
	ScriptManager::GetSingleton()->AddClassFunction<Transform>("Transform", "ScaleLocal", &Transform::ScaleLocal);
}

void NIX::Editor::Transform::SetLocalLua(LibMath::Vector3 pos, LibMath::Quaternion rot, LibMath::Vector3 scale)
{
	SetLocal(pos, rot, scale);
}

void NIX::Editor::Transform::SetWorldLua(LibMath::Vector3 pos, LibMath::Quaternion rot, LibMath::Vector3 scale)
{
	SetWorld(pos, rot, scale);
}

LibMath::Vector3 NIX::Editor::Transform::GetLocalPositionLua() const
{
	return GetLocalPosition();
}

LibMath::Quaternion NIX::Editor::Transform::GetLocalRotationLua() const
{
	return GetLocalRotation();
}

LibMath::Vector3 NIX::Editor::Transform::GetLocalScaleLua() const
{
	return GetLocalScale();
}
