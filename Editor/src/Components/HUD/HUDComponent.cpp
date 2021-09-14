#include "Components/HUD/HUDComponent.h"

#include "UI/HUDDesigner.h"

namespace NIX::Editor
{
	HUDComponent::HUDComponent(GameObject* gameObject)
		: Component(gameObject)
	{
		HUDDesigner::Add(this);
	}

	void HUDComponent::SetCompNb(int nb)
	{
		m_componentNb = nb;
	}

	int& HUDComponent::GetCompNb()
	{
		return m_componentNb;
	}

	void HUDComponent::SetZOrder(int nb)
	{
		m_ZOrder = nb;
		HUDDesigner::SetZOrder(m_componentNb);
	}

	int HUDComponent::GetZOrder() const
	{
		return m_ZOrder;
	}

	void HUDComponent::SetAnchors(float width, float height)
	{
		m_anchorWidth = width;
		m_anchorHeight = height;
	}

	void HUDComponent::SetAnchorWidth(float width)
	{
		m_anchorWidth = width;
	}

	void HUDComponent::SetAnchorHeight(float height)
	{
		m_anchorHeight = height;
	}

	void HUDComponent::SetPos(float x, float y)
	{
		m_posX = x;
		m_posY = y;
	}

	void HUDComponent::SetPosX(float x)
	{
		m_posX = x;
	}

	void HUDComponent::SetPosY(float y)
	{
		m_posY = y;
	}

	bool HUDComponent::IsClicked()
	{
		return m_isClicked;
	}

	bool HUDComponent::IsHover()
	{
		return m_isHover;
	}

	std::pair<float, float> HUDComponent::GetAnchors()
	{
		return { m_anchorWidth, m_anchorHeight };
	}

	float HUDComponent::GetAnchorWidth() const
	{
		return m_anchorWidth;
	}

	float HUDComponent::GetAnchorHeight() const
	{
		return m_anchorHeight;
	}

	std::pair<float, float> HUDComponent::GetPos()
	{
		return { m_posX, m_posY };
	}

	float HUDComponent::GetPosX() const
	{
		return m_posX;
	}

	float HUDComponent::GetPosY() const
	{
		return m_posY;
	}

	void HUDComponent::SetIsActive(bool value)
	{
		m_isActive = value;
	}

	void HUDComponent::SerializeParentClass(Core::SaveElement& element)
	{
		element.AddIntAttribute("z_order", m_ZOrder);
		element.AddFloatAttribute("pos_x", m_posX);
		element.AddFloatAttribute("pos_y", m_posY);
		element.AddFloatAttribute("anchor_width", m_anchorWidth);
		element.AddFloatAttribute("anchor_height", m_anchorHeight);
	}

	void HUDComponent::Serialize(Core::SaveElement& parent)
	{
		Core::SaveElement hudComponentElem(parent, "hud_component");

		hudComponentElem.AddIntAttribute("z_order", m_ZOrder);
		hudComponentElem.AddFloatAttribute("pos_x", m_posX);
		hudComponentElem.AddFloatAttribute("pos_y", m_posY);
		hudComponentElem.AddFloatAttribute("anchor_width", m_anchorWidth);
		hudComponentElem.AddFloatAttribute("anchor_height", m_anchorHeight);
	}

	void HUDComponent::Deserialize(const Core::SaveElement& data)
	{
		m_ZOrder = data.GetIntAttribute("z_order");
		m_posX = data.GetFloatAttribute("pos_x");
		m_posY = data.GetFloatAttribute("pos_y");
		m_anchorWidth = data.GetFloatAttribute("anchor_width");
		m_anchorHeight = data.GetFloatAttribute("anchor_height");
	}

	void HUDComponent::IsClicked(bool isClicked)
	{
		m_isClicked = isClicked;
	}

	void HUDComponent::IsHover(bool isHover)
	{
		m_isHover = isHover;
	}

	HUDComponent::~HUDComponent()
	{
		HUDDesigner::Remove(m_componentNb);
	}

	bool HUDComponent::GetIsClicked() const
	{
		return m_isClicked;
	}

	bool HUDComponent::GetIsHover() const
	{
		return m_isHover;
	}
}
