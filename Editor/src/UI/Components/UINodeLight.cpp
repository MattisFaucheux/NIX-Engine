#include "pch.h"
#include "UI/Components/UINodeLight.h"

#include "UI/UIEditor.h"
#include "UI/UI.h"
#include "GameObject.h"

#include "Components/Lights/Light.h"
#include "Light/ELightType.h"
#include "Light/Light.h"
#include <iostream>

namespace NIX::Editor
{
	void UINodeLight::UpdateComponentProperties(GameObject* obj)
	{
		m_obj = obj;

		Light* light = m_obj->GetComponent<Light>();

		if (!light)
			return;

		switch (light->GetLight()->lightType)
		{
		case Rendering::ELightType::DIRECTIONAL:
			m_type = m_types[0];
			m_lastType = Rendering::ELightType::DIRECTIONAL;
			break;
		case Rendering::ELightType::POINT:
			m_type = m_types[1];
			m_lastType = Rendering::ELightType::POINT;
			break;
		case Rendering::ELightType::SPOT:
			m_type = m_types[2];
			m_lastType = Rendering::ELightType::SPOT;
			break;
		default:
			break;
		}

		const LibMath::Vector4 ambient = light->GetLight()->ambient;

		m_ambient[0] = ambient.x;
		m_ambient[1] = ambient.y;
		m_ambient[2] = ambient.z;

		const LibMath::Vector4 diffuse = light->GetLight()->diffuse;

		m_diffuse[0] = diffuse.x;
		m_diffuse[1] = diffuse.y;
		m_diffuse[2] = diffuse.z;

		const LibMath::Vector4 specular = light->GetLight()->specular;

		m_specular[0] = specular.x;
		m_specular[1] = specular.y;
		m_specular[2] = specular.z;

		if (m_type == m_types[0])
		{
			m_obj->GetComponent<Light>()->UpdateLightType(NIX::Rendering::ELightType::DIRECTIONAL);
		}
		else
		{
			const LibMath::Vector4 attenuation = light->GetLight()->attenuation;

			m_attenuation[0] = attenuation.x;
			m_attenuation[1] = attenuation.y;
			m_attenuation[2] = attenuation.z;

			if (m_type == m_types[1])
			{
				m_obj->GetComponent<Light>()->UpdateLightType(NIX::Rendering::ELightType::POINT);
			}
			else
			{
				m_obj->GetComponent<Light>()->UpdateLightType(NIX::Rendering::ELightType::SPOT);

				m_innerCone = light->GetLight()->innerCone;
				m_outerCone = light->GetLight()->outerCone;
			}
		}
	}

	void UINodeLight::UpdateComponent()
	{
		if (!m_obj->GetComponent<Light>())
			return;

		static bool isLightOpen = true;

		if (m_lastType == Rendering::ELightType::UNDEFINED)
		{
			m_lastType = m_obj->GetComponent<Light>()->GetLight()->lightType;
		}

		if (m_lastType != m_obj->GetComponent<Light>()->GetLight()->lightType)
		{
			m_obj->GetComponent<Light>()->UpdateLightType(m_lastType);
			if (m_lastType == Rendering::ELightType::DIRECTIONAL)
			{
				m_type = m_types[0];
			}
			else if (m_lastType == Rendering::ELightType::POINT)
			{
				m_type = m_types[1];
			}
			else if (m_lastType == Rendering::ELightType::SPOT)
			{
				m_type = m_types[2];
			}

		}

		UI::GetUI()->NewChild("Light", 0.f, isLightOpen ? (m_type == m_types[0] ? 130.f : (m_type == m_types[1] ? 155.f : 205.f)) : 30.f, true);
		UI::GetUI()->SetNextItemOpen(isLightOpen);

		if (UI::GetUI()->AddNode("Light"))
		{
			if (UIEditor::GetInspectorWindow().RightClickComp(m_obj->GetComponent<Light>()))
			{
				UpdateComponentNode(isLightOpen);
			}
				
			UI::GetUI()->CloseNode();
		}
		else
		{
			UIEditor::GetInspectorWindow().RightClickComp(m_obj->GetComponent<Light>());
			isLightOpen = false;
		}

		UI::GetUI()->EndChild();
	}

	void UINodeLight::UpdateComponentNode(bool& isComponentOpen)
	{
		Light* lightComp = m_obj->GetComponent<Light>();
		if (lightComp == nullptr)
		{
			return;
		}

		if (!UIEditor::GetInspectorWindow().OpenComponentNode(isComponentOpen, "##LightTable"))
		{
			return;
		}
		
		UpdateComponentProperties(m_obj);
		
		UpdateLightType();
		UpdateAmbient(lightComp);
		UpdateDiffuse(lightComp);
		UpdateSpecular(lightComp);

		if (m_type != m_types[0])
		{
			UpdateAttenuation(lightComp);
			if (m_type == m_types[2])
			{
				UpdateInnerCone(lightComp);
				UpdateOuterCone(lightComp);
			}
		}
		UIEditor::GetInspectorWindow().CloseComponentNode();
	}

	void UINodeLight::UpdateLightType()
	{
		std::string futurType = m_type;

		UIEditor::GetInspectorWindow().SetLeftText("Type");
		if (UI::GetUI()->AddCombo("##LightType", m_types, futurType))
		{
			if (futurType == m_types[0])
			{
				m_lastType = Rendering::ELightType::DIRECTIONAL;
			}
			else if (futurType == m_types[1])
			{
				m_lastType = Rendering::ELightType::POINT;
			}
			else if (futurType == m_types[2])
			{
				m_lastType = Rendering::ELightType::SPOT;
			}
		}
	}

	void UINodeLight::UpdateAmbient(Light* lightComponent)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Ambient");
		if (UI::GetUI()->AddColorPicker3("##LightAmbient", m_ambient))
		{
			lightComponent->UpdateAmbient({ m_ambient[0], m_ambient[1], m_ambient[2] });
		}
	}

	void UINodeLight::UpdateDiffuse(Light* lightComponent)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Diffuse");
		if (UI::GetUI()->AddColorPicker3("##LightDiffuse", m_diffuse))
		{
			lightComponent->UpdateDiffuse({ m_diffuse[0], m_diffuse[1], m_diffuse[2] });
		}
	}

	void UINodeLight::UpdateSpecular(Light* lightComponent)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Specular");
		if (UI::GetUI()->AddColorPicker3("##LightSpecular", m_specular))
		{
			lightComponent->UpdateSpecular({ m_specular[0], m_specular[1], m_specular[2] });
		}
	}

	void UINodeLight::UpdateAttenuation(Light* lightComponent)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Attenuation");
		if (UI::GetUI()->AddFloat3("##LightAttenuation", m_attenuation))
		{
			lightComponent->UpdateConstantAttenuation(m_attenuation[0]);
			lightComponent->UpdateLinearAttenuation(m_attenuation[1]);
			lightComponent->UpdateQuadraticAttenuation(m_attenuation[2]);
		}
	}

	void UINodeLight::UpdateInnerCone(Light* lightComponent)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Inner Cone");
		if (UI::GetUI()->AddFloat("##LightInnerCone", m_innerCone))
		{
			lightComponent->UpdateInnerCone(m_innerCone);
		}
	}

	void UINodeLight::UpdateOuterCone(Light* lightComponent)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Outer Cone");
		if (UI::GetUI()->AddFloat("##LightOuterCone", m_outerCone))
		{
			lightComponent->UpdateOuterCone(m_outerCone);
		}
	}
}
