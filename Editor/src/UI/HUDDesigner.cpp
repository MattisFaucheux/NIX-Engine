#include "UI/HUDDesigner.h"

#include "UI/UI.h"

#include "Components/HUD/Text.h"
#include "Components/HUD/Image.h"
#include "Log/Logger.h"
#include "ResourceManager/MainResourceManager.h"

void NIX::Editor::HUDDesigner::Remove(int id)
{
	if (id >= m_components.size())
	{
		LOG(SOURCE_INFO, Core::EVerbosity::LOG_WARNING, "Vector size is : %d, id to delete is %d, vector out of range", m_components.size(), id);
		return;
	}
	
	m_components.erase(m_components.begin() + id);

	for (size_t i = id; i < m_components.size(); i++)
	{
		m_components[i]->SetCompNb((int)i);
	}
}

void NIX::Editor::HUDDesigner::Sort(HUDComponent* compChange)
{
	bool onlyOnce = true;

	int pos = 0;
	for (HUDComponent* comp : m_components)
	{
		if (comp->GetZOrder() > compChange->GetZOrder())
		{
			m_components.insert(m_components.begin() + pos, compChange);
			onlyOnce = false;
			break;
		}

		pos++;
	}

	if (onlyOnce)
	{
		m_components.push_back(compChange);
		compChange->SetCompNb((int)m_components.size() - 1);
	}
	else
	{
		for (int i = pos; i < (int)m_components.size(); i++)
		{
			m_components[i]->SetCompNb(i);
		}
	}
}

void NIX::Editor::HUDDesigner::SetZOrder(int index)
{
	HUDComponent* compChange = m_components[index];
	Remove(index);

	Sort(compChange);
}

void NIX::Editor::HUDDesigner::UpdateHUDDesigner(std::string_view label)
{
	std::pair<float, float> av = UI::GetUI()->GetContentRegionAvail();

	//Aspect Ratio 1920x1080
	std::pair<float, float> ar = { av.first / 1920.f, av.second / 1080.f };

	if (!UI::GetUI()->NewChild(label, 0.0f, 0.0f, false, false))
	{
		UI::GetUI()->EndChild();
		return;
	}

	for (HUDComponent* comp : m_components)
	{
		if (!comp->GetIsActive()) continue;

		if (dynamic_cast<Text*>(comp))
		{
			Text* text = dynamic_cast<Text*>(comp);

			int fontRatio = static_cast<int>(text->GetSize() * (ar.first + ar.second) * .5f);
			if (fontRatio <= 0) continue;

			if (!UI::GetUI()->PushFont({ text->GetFont().data(), fontRatio }))
			{
				if (!UI::GetUI()->PushFont( text->GetLast()))
				{
					continue;
				}
			}
			else
			{
				text->SetLast(text->GetFont().data(), fontRatio);
			}

			std::pair<float, float> anchors = text->GetAnchors();
			std::pair<float, float> textSize = UI::GetUI()->CalcTextSize(text->GetText());
			std::pair<float, float> pos = text->GetPos();

			textSize.first *= ar.first;
			textSize.second *= ar.second;

			UI::GetUI()->SetCursorPos(anchors.first * av.first - textSize.first * .5f + pos.first * ar.first, anchors.second * av.second - textSize.second * .5f + pos.second * ar.second);
			UI::GetUI()->AddTextColored(text->GetText(), text->GetColor());
			UI::GetUI()->PopFont();
		}
		else
		{
			Image* image = dynamic_cast<Image*>(comp);

			std::pair<float, float> anchors = image->GetAnchors();
			std::pair<float, float> imageSize = image->GetSize();
			std::pair<float, float> pos = image->GetPos();

			imageSize.first *= ar.first;
			imageSize.second *= ar.second;

			UI::GetUI()->SetCursorPos(anchors.first * av.first - imageSize.first * .5f + pos.first * ar.first, anchors.second * av.second - imageSize.second * .5f + pos.second * ar.second);
			UI::GetUI()->AddImage(MainResourceManager::GetUiTextureId(image->GetImage().data()), imageSize.first, imageSize.second);
		}

		comp->IsHover(UI::GetUI()->IsHover());
		comp->IsClicked(UI::GetUI()->IsClicked());
	}

	UI::GetUI()->EndChild();
}

void NIX::Editor::HUDDesigner::IsOpen(bool isOpen)
{
	m_isOpen = isOpen;
}

bool& NIX::Editor::HUDDesigner::IsOpen()
{
	return m_isOpen;
}
