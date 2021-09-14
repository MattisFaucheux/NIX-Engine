#include "pch.h"

#include "UI/Components/UINodeModel.h"

#include "EditorApp.h"
#include "UI/UIEditor.h"
#include "UI/UI.h"

#include "GameObject.h"
#include "Components/Model.h"

#include "ResourceManager/MeshManager.h"
#include "ResourceManager/MaterialManager.h"

namespace NIX::Editor
{
	void UINodeModel::UpdateComponentProperties(GameObject* obj)
	{
		if (obj == nullptr)
		{
			return;
		}

		m_obj = obj;
		
		Model* modelComp = m_obj->GetComponent<Model>();
		if (modelComp == nullptr)
		{
			return;
		}
		
		m_meshPath = modelComp->GetMeshPath();
		m_materialPath = modelComp->GetMaterialPath();
		m_materialName = ((std::filesystem::path)modelComp->GetMaterialPath()).stem().string();
	}

	void UINodeModel::UpdateComponent()
	{
		if (m_obj == nullptr || m_obj->GetComponent<Model>() == nullptr)
		{
			return;
		}

		static bool isModelOpen = true;
		
		UI::GetUI()->NewChild("Model", 0.0f, isModelOpen ? 93.0f : 30.0f, true);
		UI::GetUI()->SetNextItemOpen(isModelOpen);

		if (UI::GetUI()->AddNode("Model"))
		{
			if(UIEditor::GetInspectorWindow().RightClickComp(m_obj->GetComponent<Model>()))
				UpdateComponentNode(isModelOpen);
			UI::GetUI()->CloseNode();
		}
		else
		{
			UIEditor::GetInspectorWindow().RightClickComp(m_obj->GetComponent<Model>());
			isModelOpen = false;
		}
		
		UI::GetUI()->EndChild();
	}

	void UINodeModel::SetMeshDrag(std::filesystem::path path)
	{
		m_meshDrag = path.string();
	}

	void UINodeModel::SetMaterialDrag(std::filesystem::path path)
	{
		m_materialDrag = path.string();
		m_materialDragName = path.stem().string();
	}


	void UINodeModel::UpdateComponentNode(bool& isComponentOpen)
	{
		Model* modelComp = m_obj->GetComponent<Model>();
		if (modelComp == nullptr)
		{
			return;
		}

		if (!UIEditor::GetInspectorWindow().OpenComponentNode(isComponentOpen, "##ModelTable"))
		{
			return;
		}
		
		UpdateComponentProperties(m_obj);
		
		UpdateMesh(modelComp);
		UpdateMaterial(modelComp);

		UIEditor::GetInspectorWindow().CloseComponentNode();
	}

	void UINodeModel::UpdateMesh(Model* modelComponent)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Mesh");
		UI::GetUI()->AddInputText("Mesh", "Mesh path", m_meshPath, true);
		if (UI::GetUI()->DragDropTarget("DragDropMeshes"))
		{
			m_meshPath = m_meshDrag;
			GeometricTools::Mesh* newMesh = MainResourceManager::GetMeshManager()->Find(m_meshPath.c_str());

			if (newMesh != nullptr)
			{
				modelComponent->UpdateMesh(m_meshPath.c_str());
				modelComponent->SetMeshPath(m_meshPath);
			}
		}
		if (UI::GetUI()->IsClicked(1))
		{
			UI::GetUI()->OpenPopup("##RightClickedMeshPath");
		}
		int res = UI::GetUI()->AddPopup("##RightClickedMeshPath", m_rightClickMesh);
		if (res == 0)
		{
			m_meshPath = "Cube";
			modelComponent->UpdateMesh(m_meshPath.c_str());
			modelComponent->SetMeshPath(m_meshPath);
		}
		else if (res == 1)
		{
			m_meshPath = "Sphere";
			modelComponent->UpdateMesh(m_meshPath.c_str());
			modelComponent->SetMeshPath(m_meshPath);
		}
	}

	void UINodeModel::UpdateMaterial(Model* modelComponent)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Material");
		UI::GetUI()->AddImage(MainResourceManager::GetUiTextureId(m_materialPath), 25.f, 25.f);
		if (UI::GetUI()->DragDropTarget("DragDropMaterials"))
		{
			m_materialPath = m_materialDrag;
			m_materialName = m_materialDragName;
			Rendering::Material* newMat = MainResourceManager::GetMaterialManager()->Find(m_materialPath.c_str());

			if (newMat != nullptr)
			{
				modelComponent->UpdateMaterial(m_materialPath.c_str());
				modelComponent->SetMaterialPath(m_materialPath);
			}
		}
		if (m_materialPath != "Default Material" && UI::GetUI()->IsClicked(1))
		{
			UI::GetUI()->OpenPopup("##RightClickedMaterialPath");
		}
		if (UI::GetUI()->AddPopup("##RightClickedMaterialPath", m_rightClickMat) == 0)
		{
			m_materialPath = "Default Material";
			modelComponent->UpdateMaterial(m_materialPath.c_str());
			modelComponent->SetMaterialPath(m_materialPath);
		}
		UI::GetUI()->SameLine();
		UI::GetUI()->SetCursorPosY(UI::GetUI()->GetCursorPosY() + 5.f);
		UI::GetUI()->AddText(m_materialName);
	}
}

