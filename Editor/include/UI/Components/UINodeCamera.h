#pragma once
#include "UINodeComponent.h"

namespace NIX::Editor
{
	class Camera;
	class GameObject;

	class UINodeCamera : public UINodeComponent
	{
	private:
		float m_far = 0.0f;
		float m_fov = 0.0f;
		
	public:		
		void UpdateComponentProperties(GameObject* obj) override;
		
		void UpdateComponent() override;

	private:
		void UpdateComponentNode(bool& isComponentOpen) override;

		void UpdateFov(Camera* camera);
		void UpdateFar(Camera* camera);
	};
	
	
}
