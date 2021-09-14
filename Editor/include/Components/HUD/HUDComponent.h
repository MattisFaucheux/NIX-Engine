#pragma once
#include "Component.h"

namespace NIX::Editor
{
	class HUDComponent : public Component
	{
	public:

		HUDComponent(GameObject* gameObject);

		virtual void SetCompNb(int nb);
		virtual [[nodiscard]] int& GetCompNb();

		virtual void SetZOrder(int nb);
		virtual [[nodiscard]] int GetZOrder() const;

		virtual void SetAnchors(float width, float height);
		virtual void SetAnchorWidth(float width);
		virtual void SetAnchorHeight(float height);

		virtual void SetPos(float x, float y);
		virtual void SetPosX(float x);
		virtual void SetPosY(float y);

		virtual [[nodiscard]] bool IsClicked();
		virtual [[nodiscard]] bool IsHover();

		virtual [[nodiscard]] std::pair<float, float> GetAnchors();
		virtual [[nodiscard]] float GetAnchorWidth() const;
		virtual [[nodiscard]] float GetAnchorHeight() const;
		virtual [[nodiscard]] std::pair<float, float> GetPos();
		virtual [[nodiscard]] float GetPosX() const;
		virtual [[nodiscard]] float GetPosY() const;

		void SetIsActive(bool value) override;

		void SerializeParentClass(Core::SaveElement& element);

		void Serialize(Core::SaveElement& parent) override;
		void Deserialize(const  Core::SaveElement& data) override;

		//Internal Functions

		virtual void IsClicked(bool isClicked);
		virtual void IsHover(bool isHover);

		~HUDComponent();

	protected:

		virtual [[nodiscard]] bool GetIsClicked() const;
		virtual [[nodiscard]] bool GetIsHover() const;

		int m_componentNb = 0;

		int m_ZOrder = 0;

		float m_posX = 0.f;
		float m_posY = 0.f;

		float m_anchorWidth = .5f;
		float m_anchorHeight = .5f;

		bool m_isClicked = false;
		bool m_isHover = false;
	};
}