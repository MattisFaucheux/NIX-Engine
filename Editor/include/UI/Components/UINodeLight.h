#pragma once

#include <vector>
#include <string>

#include "UINodeComponent.h"

namespace NIX::Rendering 
{
	enum class ELightType;
}

namespace NIX::Editor
{
	class Light;
	class GameObject;

	class UINodeLight : public UINodeComponent
	{
	private:		
		/// Light Properties

		Rendering::ELightType m_lastType;

		std::vector<std::string> m_types = { "Directional", "Point", "Spot" };
		std::string m_type;

		float m_ambient[3] = { 0.0f, 0.0f, 0.0f };
		float m_diffuse[3] = { 0.0f, 0.0f, 0.0f };
		float m_specular[3] = { 0.0f, 0.0f, 0.0f };

		float m_attenuation[3] = { 0.0f, 0.0f, 0.0f };

		/// Spot Light Properties

		float m_innerCone;
		float m_outerCone;

		/// 

	public:		
		void UpdateComponentProperties(GameObject* obj) override;
		void UpdateComponent() override;

	private:
		void UpdateComponentNode(bool& isComponentOpen) override;

		void UpdateLightType();
		void UpdateAmbient(Light* lightComponent);
		void UpdateDiffuse(Light* lightComponent);
		void UpdateSpecular(Light* lightComponent);
		void UpdateAttenuation(Light* lightComponent);
		void UpdateInnerCone(Light* lightComponent);
		void UpdateOuterCone(Light* lightComponent);
	};
}
