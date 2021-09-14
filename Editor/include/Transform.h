#pragma once
#include "Component.h"

#include "Matrix/Matrix4.h"

class GameObject;

namespace NIX::Editor
{
	class GameObject;

	class Transform : public Component
	{
	public:
		Transform(GameObject* gameObject, LibMath::Vector3 localPos = LibMath::Vector3::Zero, LibMath::Quaternion localRot = LibMath::Quaternion::Quaternion(), LibMath::Vector3 localScale = LibMath::Vector3::One);
		Transform(GameObject* gameObject, const Transform& other);
		~Transform() = default;
		
		Transform operator-(Transform trans);

		void Serialize(Core::SaveElement& parent) override;
		void Deserialize(const Core::SaveElement& data) override;

		void SetIsActive(bool value) override;
		
		void SetLocal(LibMath::Matrix4 transform);
		void SetLocal(LibMath::Vector3 pos, LibMath::Quaternion rot, LibMath::Vector3 scale);

		void SetWorld(LibMath::Matrix4 transform);
		void SetWorld(LibMath::Vector3 pos, LibMath::Quaternion rot, LibMath::Vector3 scale);

		void UpdateWorldMatrix();

		void SetPosition(struct LibMath::Vector3 pos);
		void SetRotation(LibMath::Quaternion rot);
		void SetScale(struct LibMath::Vector3 scale);

		void SetLocalPosition(struct LibMath::Vector3 pos);
		void SetLocalRotation(LibMath::Quaternion rot);
		void SetLocalScale(struct LibMath::Vector3 scale);

		void TranslateLocal(const struct LibMath::Vector3& translation);
		void RotateLocal(const LibMath::Quaternion& rotation);
		void ScaleLocal(const struct LibMath::Vector3& scale);

		const LibMath::Vector3& GetLocalPosition() const;
		const LibMath::Quaternion& GetLocalRotation() const;
		const LibMath::Vector3& GetLocalScale() const;

		LibMath::Vector3 GetWorldPosition() const;
		LibMath::Quaternion GetWorldRotation() const;
		LibMath::Vector3 GetWorldScale() const;

		const LibMath::Matrix4& GetLocalMatrix() const;
		const LibMath::Matrix4& GetWorldMatrix() const;

		LibMath::Vector3 GetWorldForward() const;
		LibMath::Vector3 GetWorldUp() const;
		LibMath::Vector3 GetWorldRight() const;

		LibMath::Vector3 GetLocalForward() const;
		LibMath::Vector3 GetLocalUp() const;
		LibMath::Vector3 GetLocalRight() const;

		static void LuaInit();

	private:

		void SetLocalLua(LibMath::Vector3 pos, LibMath::Quaternion rot, LibMath::Vector3 scale);
		void SetWorldLua(LibMath::Vector3 pos, LibMath::Quaternion rot, LibMath::Vector3 scale);


		LibMath::Vector3 GetLocalPositionLua() const;
		LibMath::Quaternion GetLocalRotationLua() const;
		LibMath::Vector3 GetLocalScaleLua() const;

		LibMath::Vector3 m_localPosition;
		LibMath::Quaternion m_localRotation;
		LibMath::Vector3 m_localScale;

		LibMath::Matrix4 m_localMatrix;
		LibMath::Matrix4 m_worldMatrix;
	};
}
