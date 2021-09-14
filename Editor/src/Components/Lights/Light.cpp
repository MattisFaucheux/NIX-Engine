#include "Components/Lights/Light.h"


#include "EditorApp.h"
#include "Transform.h"
#include "Light/Light.h"
#include "Vector/Vector4.h"

#include "Scripting/ScriptManager.h"
#include "Scripting/LuaLightType.h"

namespace NIX::Editor
{
	Light::Light(GameObject* gameObject)
		: Component(gameObject)
	{
		m_light = EditorApp::GetRenderingContext().GetLightsManager()->AddLight(Rendering::ELightType::SPOT);
	}

	Light::Light(GameObject* gameObject, Rendering::ELightType lightType, Transform transform)
		: Component(gameObject)
	{
		m_light = EditorApp::GetRenderingContext().GetLightsManager()->AddLight(lightType);

		UpdatePosition(transform.GetWorldPosition());
		UpdateDirection(transform.GetWorldForward());
	}

	Light::Light(GameObject* gameObject, const Light& other)
		: Component(gameObject)
	{
		m_light = EditorApp::GetRenderingContext().GetLightsManager()->AddLight(other.GetLight()->lightType);
		
		m_light->ambient = other.GetLight()->ambient;
		m_light->diffuse = other.GetLight()->diffuse;
		m_light->specular = other.GetLight()->specular;
		
		m_light->position = other.GetLight()->position;
		m_light->direction = other.GetLight()->direction;

		m_light->attenuation = other.GetLight()->attenuation;
		m_light->innerCone = other.GetLight()->innerCone;
		m_light->outerCone = other.GetLight()->outerCone;

		m_light->isActive = other.GetLight()->isActive;

		SetIsActive(gameObject->IsActive());
		
	}

	Light::~Light()
	{
		EditorApp::GetRenderingContext().GetLightsManager()->DestroyLight(m_light);
	}

	void Light::Serialize(Core::SaveElement& parent)
	{
		Core::SaveElement lightElem(parent, "light");

		lightElem.AddVec4Element("ambient", m_light->ambient);
		lightElem.AddVec4Element("diffuse", m_light->diffuse);
		lightElem.AddVec4Element("specular", m_light->specular);
		lightElem.AddVec4Element("position", m_light->position);
		lightElem.AddVec4Element("direction", m_light->direction);

		lightElem.AddVec4Element("attenuation", m_light->attenuation);

		lightElem.AddIntAttribute("light_type", static_cast<int>(m_light->lightType));
		lightElem.AddFloatAttribute("inner_cone", m_light->innerCone);
		lightElem.AddFloatAttribute("outer_cone", m_light->outerCone);
	}

	void Light::Deserialize(const Core::SaveElement& data)
	{
		UpdateAmbient(data.GetVector4Element("ambient"));
		UpdateDiffuse(data.GetVector4Element("diffuse"));
		UpdateSpecular(data.GetVector4Element("specular"));
		UpdatePosition(data.GetVector4Element("position"));
		UpdateDirection(data.GetVector4Element("specular"));

		UpdateConstantAttenuation(data.GetChild("attenuation").second.GetFloatAttribute("x"));
		UpdateLinearAttenuation(data.GetChild("attenuation").second.GetFloatAttribute("y"));
		UpdateQuadraticAttenuation(data.GetChild("attenuation").second.GetFloatAttribute("z"));

		UpdateLightType(static_cast<Rendering::ELightType>(data.GetIntAttribute("light_type")));
		UpdateInnerCone(data.GetFloatAttribute("inner_cone"));
		UpdateOuterCone(data.GetFloatAttribute("outer_cone"));

	}

	void Light::SetIsActive(bool value)
	{
		m_isActive = value;

		m_light->isActive = value;
	}


	void Light::UpdateLightType(Rendering::ELightType lightType)
	{
		if (m_light != nullptr)
		{
			m_light->lightType = lightType;
		}
	}

	void Light::UpdateAmbient(const LibMath::Vector3& ambient)
	{
		if (m_light != nullptr)
		{
			m_light->ambient = LibMath::Vector4(ambient.x, ambient.y, ambient.z, 0.0f);
		}
	}

	void Light::UpdateDiffuse(const LibMath::Vector3& diffuse)
	{
		if (m_light != nullptr)
		{
			m_light->diffuse = LibMath::Vector4(diffuse.x, diffuse.y, diffuse.z, 0.0f);
		}
	}

	void Light::UpdateSpecular(const LibMath::Vector3& specular)
	{
		if (m_light != nullptr)
		{
			m_light->specular = LibMath::Vector4(specular.x, specular.y, specular.z, 0.0f);
		}
	}

	void Light::UpdatePosition(const LibMath::Vector3& position)
	{
		if (m_light != nullptr)
		{
			m_light->position = LibMath::Vector4(position.x, position.y, position.z, 0.0f);
		}
	}

	void Light::UpdateDirection(const LibMath::Vector3& direction)
	{
		if (m_light != nullptr)
		{
			m_light->direction = LibMath::Vector4(direction.x, direction.y, direction.z, 0.0f);
		}
	}

	void Light::UpdateAmbient(const LibMath::Vector4& ambient)
	{
		if (m_light != nullptr)
		{
			m_light->ambient = LibMath::Vector4(ambient.x, ambient.y, ambient.z, ambient.w);
		}
	}

	void Light::UpdateDiffuse(const LibMath::Vector4& diffuse)
	{
		if (m_light != nullptr)
		{
			m_light->diffuse = LibMath::Vector4(diffuse.x, diffuse.y, diffuse.z, diffuse.w);
		}
	}

	void Light::UpdateSpecular(const LibMath::Vector4& specular)
	{
		if (m_light != nullptr)
		{
			m_light->specular = LibMath::Vector4(specular.x, specular.y, specular.z, specular.w);
		}
	}

	void Light::UpdatePosition(const LibMath::Vector4& position)
	{
		if (m_light != nullptr)
		{
			m_light->position = LibMath::Vector4(position.x, position.y, position.z, position.w);
		}
	}

	void Light::UpdateDirection(const LibMath::Vector4& direction)
	{
		if (m_light != nullptr)
		{
			m_light->direction = LibMath::Vector4(direction.x, direction.y, direction.z, direction.w);
		}
	}
	
	void Light::UpdateInnerCone(float innerCone)
	{
		if (m_light != nullptr)
		{
			m_light->innerCone = innerCone;
		}
	}

	void Light::UpdateOuterCone(float outerCone)
	{
		if (m_light != nullptr)
		{
			m_light->outerCone = outerCone;
		}
	}

	void Light::UpdateConstantAttenuation(float constantAttenuation)
	{
		if (m_light != nullptr)
		{
			m_light->attenuation.x = constantAttenuation;
		}
	}

	void Light::UpdateLinearAttenuation(float linearAttenuation)
	{
		if (m_light != nullptr)
		{
			m_light->attenuation.y = linearAttenuation;
		}
	}

	void Light::UpdateQuadraticAttenuation(float quadraticAttenuation)
	{
		if (m_light != nullptr)
		{
			m_light->attenuation.z = quadraticAttenuation;
		}
	}

	Rendering::Light* Light::GetLight() const
	{
		return m_light;
	}

	void Light::LuaInit()
	{
		LuaLightType::Init();

		ScriptManager::GetSingleton()->AddClass<Light, GameObject*>("Light");

		ScriptManager::GetSingleton()->AddClassProperty<Light>("Light", "IsActive", &Light::GetIsActive, &Light::SetIsActive);
		ScriptManager::GetSingleton()->AddClassData<Light>("Light", "GameObject", &Light::m_gameObject, false);
		ScriptManager::GetSingleton()->AddClassFunction<Light>("Light", "GetGameObject", &Light::GetParent);

		ScriptManager::GetSingleton()->AddClassProperty<Light>("Light", "Ambient", &Light::GetAmbient, &Light::SetAmbientVec4);
		ScriptManager::GetSingleton()->AddClassProperty<Light>("Light", "Diffuse", &Light::GetDiffuse, &Light::SetDiffuseVec4);
		ScriptManager::GetSingleton()->AddClassProperty<Light>("Light", "Specular", &Light::GetSpecular, &Light::SetSpecularVec4);

		ScriptManager::GetSingleton()->AddClassFunction<Light>("Light", "SetAmbient", &Light::SetAmbientVec3);
		ScriptManager::GetSingleton()->AddClassFunction<Light>("Light", "SetDiffuse", &Light::SetDiffuseVec3);
		ScriptManager::GetSingleton()->AddClassFunction<Light>("Light", "SetSpecular", &Light::SetSpecularVec3);

		ScriptManager::GetSingleton()->AddClassProperty<Light>("Light", "InnerCone", &Light::GetInnerCone, &Light::UpdateInnerCone);
		ScriptManager::GetSingleton()->AddClassProperty<Light>("Light", "OuterCone", &Light::GetOuterCone, &Light::UpdateOuterCone);

		ScriptManager::GetSingleton()->AddClassProperty<Light>("Light", "ConstantAttenuation", &Light::GetConstantAttenuation, &Light::UpdateConstantAttenuation);
		ScriptManager::GetSingleton()->AddClassProperty<Light>("Light", "LinearAttenuation", &Light::GetLinearAttenuation, &Light::UpdateLinearAttenuation);
		ScriptManager::GetSingleton()->AddClassProperty<Light>("Light", "QuadraticAttenuation", &Light::GetQuadraticAttenuation, &Light::UpdateQuadraticAttenuation);

		ScriptManager::GetSingleton()->AddClassFunction<Light>("Light", "SetLightType", &Light::SetLightType);
	}

	void Light::SetAmbientVec3(const LibMath::Vector3& ambient)
	{
		UpdateAmbient(ambient);
	}

	void Light::SetDiffuseVec3(const LibMath::Vector3& diffuse)
	{
		UpdateDiffuse(diffuse);
	}

	void Light::SetSpecularVec3(const LibMath::Vector3& specular)
	{
		UpdateSpecular(specular);
	}

	void Light::SetAmbientVec4(const LibMath::Vector4& ambient)
	{
		UpdateAmbient(ambient);
	}

	void Light::SetDiffuseVec4(const LibMath::Vector4& diffuse)
	{
		UpdateDiffuse(diffuse);
	}

	void Light::SetSpecularVec4(const LibMath::Vector4& specular)
	{
		UpdateSpecular(specular);
	}

	LibMath::Vector4 Light::GetAmbient() const
	{
		return GetLight()->ambient;
	}

	LibMath::Vector4 Light::GetDiffuse() const
	{
		return GetLight()->diffuse;
	}

	LibMath::Vector4 Light::GetSpecular() const
	{
		return GetLight()->specular;
	}

	LibMath::Vector4 Light::GetInnerCone() const
	{
		return GetLight()->innerCone;
	}

	LibMath::Vector4 Light::GetOuterCone() const
	{
		return GetLight()->outerCone;
	}

	float Light::GetConstantAttenuation() const
	{
		return GetLight()->attenuation.x;
	}

	float Light::GetLinearAttenuation() const
	{
		return GetLight()->attenuation.y;
	}

	float Light::GetQuadraticAttenuation() const
	{
		return GetLight()->attenuation.z;
	}

	void Light::SetLightType(std::string lightType)
	{
		UpdateLightType(LuaLightType::GetLightType(lightType));
	}
}
