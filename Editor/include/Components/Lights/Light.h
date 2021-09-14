#pragma once
#include <Vector/Vector3.h>

#include "Component.h"

namespace NIX {
	namespace Rendering {
		enum class ELightType;
		class Context;
		struct Light;
	}
}

namespace NIX::Editor
{
	class Transform;

	class Light : public Component
	{
	private:
		Rendering::Light* m_light = nullptr;
	public:
		Light(GameObject* gameObject);
		Light(GameObject* gameObject, Rendering::ELightType lightType, Transform transform);
		Light(GameObject* gameObject, const Light& other);
		~Light();

		void Serialize(Core::SaveElement& parent) override;
		void Deserialize(const Core::SaveElement& data) override;

		void SetIsActive(bool value) override;
		
		void UpdateLightType(Rendering::ELightType lightType);
		void UpdateAmbient(const LibMath::Vector3& ambient);
		void UpdateDiffuse(const LibMath::Vector3& diffuse);
		void UpdateSpecular(const LibMath::Vector3& specular);
		void UpdatePosition(const LibMath::Vector3& position);
		void UpdateDirection(const LibMath::Vector3& direction);

		void UpdateAmbient(const LibMath::Vector4& ambient);
		void UpdateDiffuse(const LibMath::Vector4& diffuse);
		void UpdateSpecular(const LibMath::Vector4& specular);
		void UpdatePosition(const LibMath::Vector4& position);
		void UpdateDirection(const LibMath::Vector4& direction);
		
		void UpdateInnerCone(float innerCone);
		void UpdateOuterCone(float outerCone);
		void UpdateConstantAttenuation(float constantAttenuation);
		void UpdateLinearAttenuation(float linearAttenuation);
		void UpdateQuadraticAttenuation(float quadraticAttenuation);

		Rendering::Light* GetLight() const;

		static void LuaInit();

	private:
		void SetAmbientVec3(const LibMath::Vector3& ambient);
		void SetDiffuseVec3(const LibMath::Vector3& diffuse);
		void SetSpecularVec3(const LibMath::Vector3& specular);

		void SetAmbientVec4(const LibMath::Vector4& ambient);
		void SetDiffuseVec4(const LibMath::Vector4& diffuse);
		void SetSpecularVec4(const LibMath::Vector4& specular);

		LibMath::Vector4 GetAmbient() const;
		LibMath::Vector4 GetDiffuse() const;
		LibMath::Vector4 GetSpecular() const;
		LibMath::Vector4 GetInnerCone() const;
		LibMath::Vector4 GetOuterCone() const;
		float GetConstantAttenuation() const;
		float GetLinearAttenuation() const;
		float GetQuadraticAttenuation() const;

		void SetLightType(std::string lightType);

	};
}
