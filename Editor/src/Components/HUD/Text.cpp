#include "Components/HUD/Text.h"
#include "GameObject.h"

#include "Scripting/ScriptManager.h"

namespace NIX::Editor
{
	Text::Text(GameObject* gameObject, std::string_view text, LibMath::Vector4 color)
		: HUDComponent(gameObject)
	{
		m_text = text;
		m_color = color;
		m_font = "Default Font";
		m_size = 16;

		m_lastFont = "";
		m_lastSize = 0;
	}

	Text::Text(GameObject* gameObject, const Text& other)
		: HUDComponent(gameObject)
	{
		m_text = other.m_text;
		m_color = other.m_color;
		m_font = other.m_font;
		m_size = other.m_size;

		m_lastFont = other.m_lastFont;
		m_lastSize = other.m_lastSize;

		m_posX = other.m_posX;
		m_posY = other.m_posY;
		m_anchorWidth = other.m_anchorWidth;
		m_anchorHeight = other.m_anchorHeight;

		
		SetZOrder(other.m_ZOrder);
		SetIsActive(gameObject->IsActive());
	}

	void Text::SetText(std::string_view text)
	{
		m_text = text;
	}

	std::string_view Text::GetText() const
	{
		return m_text;
	}

	void Text::SetFont(std::string_view path)
	{
		m_font = path;
	}

	std::string_view Text::GetFont() const
	{
		return m_font;
	}

	void Text::SetSize(int size)
	{
		m_size = size;
	}

	int Text::GetSize() const
	{
		return m_size;
	}

	void Text::SetColor(LibMath::Vector4 color)
	{
		m_color = color;
	}

	LibMath::Vector4 Text::GetColor() const
	{
		return m_color;
	}

	void Text::Serialize(Core::SaveElement& parent)
	{
		Core::SaveElement textElem(parent, "text");

		SerializeParentClass(textElem);

		textElem.AddStringAttribute("text", m_text.data());
		textElem.AddStringAttribute("font", m_font.data());
		textElem.AddIntAttribute("size", m_size);
		textElem.AddVec4Element("color", m_color);
	}

	void Text::Deserialize(const  Core::SaveElement& data)
	{
		HUDComponent::Deserialize(data);

		m_text = data.GetStringAttribute("text");
		m_font = data.GetStringAttribute("font");
		m_size = data.GetIntAttribute("size");
		m_color = data.GetVector4Element("color");

		m_lastFont = "";
		m_lastSize = 0;
	}

	void Text::SetLast(std::string lastFont, int lastSize)
	{
		m_lastFont = lastFont;
		m_lastSize = lastSize;
	}

	std::pair<std::string, int> Text::GetLast()
	{
		return {m_lastFont, m_lastSize};
	}

	void Text::LuaInit()
	{
		ScriptManager::GetSingleton()->AddClass<Text, GameObject*, std::string, LibMath::Vector4>("Text");

		ScriptManager::GetSingleton()->AddClassProperty<Text>("Text", "IsActive", &Text::GetIsActive, &Text::SetIsActive);
		ScriptManager::GetSingleton()->AddClassData<Text>("Text", "GameObject", &Text::m_gameObject, false);
		ScriptManager::GetSingleton()->AddClassFunction<Text>("Text", "GetGameObject", &Text::GetParent);

		ScriptManager::GetSingleton()->AddClassProperty<Text>("Text", "ZOrder", &Text::GetZOrder, &Text::SetZOrder);
		ScriptManager::GetSingleton()->AddClassProperty<Text>("Text", "AnchorWidth", &Text::GetAnchorWidth, &Text::SetAnchorWidth);
		ScriptManager::GetSingleton()->AddClassProperty<Text>("Text", "AnchorHeight", &Text::GetAnchorHeight, &Text::SetAnchorHeight);
		ScriptManager::GetSingleton()->AddClassFunction<Text>("Text", "SetAnchors", &Text::SetAnchors);
		ScriptManager::GetSingleton()->AddClassProperty<Text>("Text", "PosX", &Text::GetPosX, &Text::SetPosX);
		ScriptManager::GetSingleton()->AddClassProperty<Text>("Text", "PosY", &Text::GetPosY, &Text::SetPosY);
		ScriptManager::GetSingleton()->AddClassFunction<Text>("Text", "SetPos", &Text::SetPos);
		ScriptManager::GetSingleton()->AddClassFunction<Text>("Text", "IsClicked", &Text::GetIsClicked);
		ScriptManager::GetSingleton()->AddClassFunction<Text>("Text", "IsHover", &Text::GetIsHover);

		ScriptManager::GetSingleton()->AddClassProperty<Text>("Text", "Text", &Text::LuaGetText, &Text::LuaSetText);
		ScriptManager::GetSingleton()->AddClassProperty<Text>("Text", "Font", &Text::LuaGetFont, &Text::LuaSetFont);
		ScriptManager::GetSingleton()->AddClassProperty<Text>("Text", "Size", &Text::GetSize, &Text::SetSize);
		ScriptManager::GetSingleton()->AddClassProperty<Text>("Text", "Color", &Text::GetColor, &Text::SetColor);

	}

	void Text::LuaSetText(std::string text)
	{
		SetText(std::string_view(text));
	}

	std::string Text::LuaGetText() const
	{
		return m_text;
	}

	void Text::LuaSetFont(std::string font)
	{
		SetFont(std::string_view(font));
	}

	std::string Text::LuaGetFont() const
	{
		return m_font;
	}
}
