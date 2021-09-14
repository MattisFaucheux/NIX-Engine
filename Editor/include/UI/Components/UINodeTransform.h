#pragma once
#include "UINodeComponent.h"

#include <vector>
#include <string>

namespace NIX::Editor
{
	class Transform;
	class GameObject;

	class UINodeTransform : public UINodeComponent
	{
	private:
		float m_vecPosition[3] = { 0.0f, 0.0f, 0.0f };
		float m_vecRotation[3] = { 0.0f, 0.0f, 0.0f };
		float m_vecScale[3] = { 0.0f, 0.0f, 0.0f };

		std::vector<std::string> m_rightClick = { "Reset" };

	public:
		void UpdateComponentProperties(GameObject* obj) override;

		void UpdateComponent() override;

	private:
		void UpdateComponentNode(bool& isComponentOpen) override;

		void UpdatePosition(Transform* transformComponent);
		void UpdateRotation(Transform* transformComponent);
		void UpdateScale(Transform* transformComponent);

		void RightClick();
	};
}
