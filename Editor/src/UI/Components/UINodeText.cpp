#include "UI/Components/UINodeText.h"
#include "Components/HUD/Text.h"
#include "GameObject.h"
#include "UI/UIEditor.h"
#include "UI/UI.h"
#include <filesystem>

namespace NIX::Editor
{
	void UINodeText::UpdateComponentProperties(GameObject* obj)
	{
		if (obj == nullptr)
		{
			return;
		}

		m_obj = obj;
		
		Text* text = m_obj->GetComponent<Text>();

		if (text)
		{
			m_text = text->GetText();

			m_fontPath = text->GetFont();

			m_size = text->GetSize();

			m_ZOrder = text->GetZOrder();

			std::pair<float, float> anchors = text->GetAnchors();
			m_anchors[0] = anchors.first;
			m_anchors[1] = anchors.second;

			std::pair<float, float> pos = text->GetPos();
			m_pos[0] = pos.first;
			m_pos[1] = pos.second;

			LibMath::Vector4 color = text->GetColor();
			m_color[0] = color.x;
			m_color[1] = color.y;
			m_color[2] = color.z;
			m_color[3] = color.w;
		}
	}

	void UINodeText::UpdateComponent()
	{
		if (m_obj == nullptr || m_obj->GetComponent<Text>() == nullptr)
		{
			return;
		}
		
		static bool isTextOpen = true;

		UI::GetUI()->NewChild("Text", 0.f, isTextOpen ? 197.f : 30.f, true);
		UI::GetUI()->SetNextItemOpen(isTextOpen);

		if (UI::GetUI()->AddNode("Text"))
		{
			if (UIEditor::GetInspectorWindow().RightClickComp(m_obj->GetComponent<Text>()))
			{
				UpdateComponentNode(isTextOpen);
			}
			UI::GetUI()->CloseNode();
		}
		else
		{
			UIEditor::GetInspectorWindow().RightClickComp(m_obj->GetComponent<Text>());
			isTextOpen = false;
		}

		UI::GetUI()->EndChild();
	}

	void UINodeText::SetFontDrag(std::filesystem::path path)
	{
		m_fontDrag = path.string();
	}

	void UINodeText::UpdateComponentNode(bool& isComponentOpen)
	{
		Text* textComp = m_obj->GetComponent<Text>();
		if (textComp == nullptr)
		{
			return;
		}

		if (!UIEditor::GetInspectorWindow().OpenComponentNode(isComponentOpen, "##HUDTextTable"))
		{
			return;
		}
		
		UpdateComponentProperties(m_obj);

		UpdatePos(textComp);
		UpdateAnchors(textComp);
		UpdateZOrder(textComp);
		UpdateText(textComp);
		UpdateColor(textComp);
		UpdateFont(textComp);
		UpdateSize(textComp);

		UIEditor::GetInspectorWindow().CloseComponentNode();
	}

	void UINodeText::UpdatePos(Text* text)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Pos");
		if (UI::GetUI()->AddFloat2("##HUDTextPos", m_pos))
		{
			text->SetPos(m_pos[0], m_pos[1]);
		}
	}

	void UINodeText::UpdateAnchors(Text* text)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Anchors");
		if (UI::GetUI()->AddFloat2("##HUDTextAnchors", m_anchors))
		{
			text->SetAnchors(m_anchors[0], m_anchors[1]);
		}
	}

	void UINodeText::UpdateZOrder(Text* text)
	{
		UIEditor::GetInspectorWindow().SetLeftText("ZOrder");
		if (UI::GetUI()->AddInt("##HUDTextZOrder", m_ZOrder))
		{
			text->SetZOrder(m_ZOrder);
		}
	}

	void UINodeText::UpdateText(Text* text)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Text");
		if (UI::GetUI()->AddInputText("##HUDTextText", "Type here...", m_text))
		{
			text->SetText(m_text);
		}
	}

	void UINodeText::UpdateColor(Text* text)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Color");
		if (UI::GetUI()->AddColorPicker4("##HUDTextColor", m_color))
		{
			text->SetColor({ m_color[0], m_color[1], m_color[2], m_color[3] });
		}
	}

	void UINodeText::UpdateFont(Text* text)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Font");
		UI::GetUI()->AddInputText("Font", "Font path", m_fontPath, true);
		if (UI::GetUI()->DragDropTarget("DragDropFonts"))
		{
			m_fontPath = m_fontDrag;
			text->SetFont(m_fontPath);
		}
		if (m_fontPath != "Default Font" && UI::GetUI()->IsClicked(1))
		{
			UI::GetUI()->OpenPopup("##RightClickedTextFont");
		}
		if (UI::GetUI()->AddPopup("##RightClickedTextFont", m_rightClick) == 0)
		{
			m_fontPath = "Default Font";
			text->SetFont(m_fontPath);
		}
	}

	void UINodeText::UpdateSize(Text* text)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Size");
		if (UI::GetUI()->AddInt("##HUDTextSize", m_size, true))
		{
			text->SetSize(m_size);
		}
	}
}
