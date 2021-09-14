#include "UI/Components/UINodeImage.h"
#include "Components/HUD/Image.h"
#include "GameObject.h"

#include <filesystem>



#include "ResourceManager/MainResourceManager.h"
#include "UI/UIEditor.h"
#include "UI/UI.h"

namespace NIX::Editor
{
	class Image;

	void UINodeImage::UpdateComponentProperties(GameObject* obj)
	{
		if (obj == nullptr)
		{
			return;
		}

		m_obj = obj;
		
		Image* image = m_obj->GetComponent<Image>();

		if (image)
		{
			m_image = image->GetImage();

			m_ZOrder = image->GetZOrder();

			std::pair<float, float> anchors = image->GetAnchors();
			m_anchors[0] = anchors.first;
			m_anchors[1] = anchors.second;

			std::pair<float, float> pos = image->GetPos();
			m_pos[0] = pos.first;
			m_pos[1] = pos.second;

			std::pair<float, float> size = image->GetSize();
			m_sizeImage[0] = size.first;
			m_sizeImage[1] = size.second;

			m_imagePath = static_cast<std::filesystem::path>(image->GetImage()).string();
			m_imageName = static_cast<std::filesystem::path>(image->GetImage()).stem().string();
		}
	}

	void UINodeImage::UpdateComponent()
	{
		if (m_obj == nullptr || m_obj->GetComponent<Image>() == nullptr)
		{
			return;
		}
		
		static bool isImageOpen = true;

		UI::GetUI()->NewChild("Image", 0.f, isImageOpen ? 162.f : 30.f, true);
		UI::GetUI()->SetNextItemOpen(isImageOpen);

		if (UI::GetUI()->AddNode("Image"))
		{
			if (UIEditor::GetInspectorWindow().RightClickComp(m_obj->GetComponent<Image>()))
			{
				UpdateComponentNode(isImageOpen);
			}
				
			UI::GetUI()->CloseNode();
		}
		else
		{
			UIEditor::GetInspectorWindow().RightClickComp(m_obj->GetComponent<Image>());
			isImageOpen = false;
		}

		UI::GetUI()->EndChild();
	}

	void UINodeImage::SetImageDrag(std::filesystem::path path)
	{
		m_imageDrag = path.string();
		m_imageDragName = path.stem().string();
	}

	void UINodeImage::UpdateComponentNode(bool& isComponentOpen)
	{
		Image* imageComp = m_obj->GetComponent<Image>();
		if (imageComp == nullptr)
		{
			return;
		}

		if (!UIEditor::GetInspectorWindow().OpenComponentNode(isComponentOpen, "##HUDImageTable"))
		{
			return;
		}
		
		UpdateComponentProperties(m_obj);
		
		UpdatePos(imageComp);
		UpdateAnchors(imageComp);
		UpdateZOrder(imageComp);
		UpdateSize(imageComp);
		UpdateImage(imageComp);

		UIEditor::GetInspectorWindow().CloseComponentNode();
		
	}

	void UINodeImage::UpdatePos(Image* imageComponent)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Pos");
		if (UI::GetUI()->AddFloat2("##HUDImagePos", m_pos))
		{
			imageComponent->SetPos(m_pos[0], m_pos[1]);
		}
	}

	void UINodeImage::UpdateAnchors(Image* imageComponent)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Anchors");
		if (UI::GetUI()->AddFloat2("##HUDImageAnchors", m_anchors))
		{
			imageComponent->SetAnchors(m_anchors[0], m_anchors[1]);
		}
	}

	void UINodeImage::UpdateZOrder(Image* imageComponent)
	{
		UIEditor::GetInspectorWindow().SetLeftText("ZOrder");
		if (UI::GetUI()->AddInt("##HUDImageZOrder", m_ZOrder))
		{
			imageComponent->SetZOrder(m_ZOrder);
		}
	}

	void UINodeImage::UpdateSize(Image* imageComponent)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Size");
		if (UI::GetUI()->AddFloat2("##HUDImageSize", m_sizeImage))
		{
			imageComponent->SetSize(m_sizeImage[0], m_sizeImage[1]);
		}
	}

	void UINodeImage::UpdateImage(Image* imageComponent)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Image");
		UI::GetUI()->AddImage(MainResourceManager::GetUiTextureId(m_imagePath), 25.f, 25.f);
		if (UI::GetUI()->DragDropTarget("DragDropImages"))
		{
			m_imagePath = m_imageDrag;
			m_imageName = m_imageDragName;
			imageComponent->SetImage(m_imagePath);
		}
		if (m_imagePath != "Default Texture" && UI::GetUI()->IsClicked(1))
		{
			UI::GetUI()->OpenPopup("##RightClickedImagePath");
		}
		if (UI::GetUI()->AddPopup("##RightClickedImagePath", m_rightClick) == 0)
		{
			m_imagePath = "Default Texture";
			imageComponent->SetImage(m_imagePath);
		}
		UI::GetUI()->SameLine();
		UI::GetUI()->SetCursorPosY(UI::GetUI()->GetCursorPosY() + 5.f);
		UI::GetUI()->AddText(m_imageName);
	}
}
