#include "Components/HUD/Image.h"
#include "GameObject.h"

#include "Scripting/ScriptManager.h"

namespace NIX::Editor
{
	Image::Image(GameObject* gameObject, std::string_view path, float width, float height)
		: HUDComponent(gameObject)
	{
		m_path = path;
		m_width = width;
		m_height = height;
	}

	Image::Image(GameObject* gameObject, const Image& other)
		: HUDComponent(gameObject)
	{
		m_path = other.m_path;
		m_width = other.m_width;
		m_height = other.m_height;

		m_posX = other.m_posX;
		m_posY = other.m_posY;
		m_anchorWidth = other.m_anchorWidth;
		m_anchorHeight = other.m_anchorHeight;

		SetZOrder(other.m_ZOrder);
		SetIsActive(gameObject->IsActive());
	}

	void Image::SetImage(std::string_view path)
	{
		m_path = path;
	}

	std::string_view Image::GetImage() const
	{
		return m_path;
	}

	void Image::SetSize(float width, float height)
	{
		m_width = width;
		m_height = height;
	}

	std::pair<float, float> Image::GetSize()
	{
		return { m_width, m_height };
	}

	void Image::SetWidth(float width)
	{
		m_width = width;
	}

	float Image::GetWidth() const
	{
		return m_width;
	}

	void Image::SetHeight(float height)
	{
		m_height = height;
	}

	float Image::GetHeight() const
	{
		return m_height;
	}

	void Image::Serialize(Core::SaveElement& parent)
	{
		Core::SaveElement imageElem(parent, "image");

		SerializeParentClass(imageElem);

		imageElem.AddStringAttribute("path", m_path.data());
		imageElem.AddFloatAttribute("width", m_width);
		imageElem.AddFloatAttribute("height", m_height);
	}

	void Image::Deserialize(const  Core::SaveElement& data)
	{
		HUDComponent::Deserialize(data);

		m_path = data.GetStringAttribute("path");
		m_width = data.GetFloatAttribute("width");
		m_height = data.GetFloatAttribute("height");
	}

	void Image::LuaInit()
	{
		ScriptManager::GetSingleton()->AddClass<Image, GameObject*, std::string, float, float>("Image");

		ScriptManager::GetSingleton()->AddClassProperty<Image>("Image", "IsActive", &Image::GetIsActive, &Image::SetIsActive);
		ScriptManager::GetSingleton()->AddClassData<Image>("Image", "GameObject", &Image::m_gameObject, false);
		ScriptManager::GetSingleton()->AddClassFunction<Image>("Image", "GetGameObject", &Image::GetParent);

		ScriptManager::GetSingleton()->AddClassProperty<Image>("Image", "ZOrder", &Image::GetZOrder, &Image::SetZOrder);
		ScriptManager::GetSingleton()->AddClassProperty<Image>("Image", "AnchorWidth", &Image::GetAnchorWidth, &Image::SetAnchorWidth);
		ScriptManager::GetSingleton()->AddClassProperty<Image>("Image", "AnchorHeight", &Image::GetAnchorHeight, &Image::SetAnchorHeight);
		ScriptManager::GetSingleton()->AddClassFunction<Image>("Image", "SetAnchors", &Image::SetAnchors);
		ScriptManager::GetSingleton()->AddClassProperty<Image>("Image", "PosX", &Image::GetPosX, &Image::SetPosX);
		ScriptManager::GetSingleton()->AddClassProperty<Image>("Image", "PosY", &Image::GetPosY, &Image::SetPosY);
		ScriptManager::GetSingleton()->AddClassFunction<Image>("Image", "SetPos", &Image::SetPos);
		ScriptManager::GetSingleton()->AddClassFunction<Image>("Image", "IsClicked", &Image::GetIsClicked);
		ScriptManager::GetSingleton()->AddClassFunction<Image>("Image", "IsHover", &Image::GetIsHover);

		ScriptManager::GetSingleton()->AddClassProperty<Image>("Image", "Image", &Image::GetImage, &Image::SetImage);
		ScriptManager::GetSingleton()->AddClassProperty<Image>("Image", "Width", &Image::GetWidth, &Image::SetWidth);
		ScriptManager::GetSingleton()->AddClassProperty<Image>("Image", "Height", &Image::GetHeight, &Image::SetHeight);
		ScriptManager::GetSingleton()->AddClassFunction<Image>("Image", "SetSize", &Image::SetSize);
	}
}
