#include "pch.h"
#include "UI/ContentsWindow.h"

#include "UI/UI.h"

#include "SceneManager.h"
#include "UI/UIEditor.h"
#include "GameObject.h"

#include "Components/CollisionBox.h"
#include "Components/CollisionSphere.h"

void NIX::Editor::ContentsWindow::SetPopup()
{
	int res = -1;

	if (m_isRightClickedRoot)
		res = UI::GetUI()->AddPopup("##RightClickedContentsTreeRoot", m_rightClickRoot);

	if (res != -1)
		m_isRightClickedRoot = false;

	switch (res)
	{
	case 0:
		m_newObjType = OBJ_TYPE::EMPTY;
		break;
	case 1:
		m_newObjType = OBJ_TYPE::CUBE;
		break;
	case 2:
		m_newObjType = OBJ_TYPE::SPHERE;
		break;
	default:
		break;
	}

	res = -1;

	if (m_isRightClicked)
	{
		if (m_isRightClickedOnce)
		{
			UI::GetUI()->OpenPopup("##RightClickedContentsTree");
			m_isRightClickedOnce = false;
		}

		res = UI::GetUI()->AddPopup("##RightClickedContentsTree", m_rightClick);
	}

	if (res != -1)
	{
		m_isRightClicked = false;
		m_isRightClickedOnce = true;
	}

	switch (res)
	{
	case 0:
		m_newObjType = OBJ_TYPE::DUPLICATE;
		break;
	case 1:
		m_isDelete = true;
		break;
	case 3:
		m_newObjType = OBJ_TYPE::EMPTY;
		break;
	case 4:
		m_newObjType = OBJ_TYPE::CUBE;
		break;
	case 5:
		m_newObjType = OBJ_TYPE::SPHERE;
		break;
	default:
		break;
	}
}

void NIX::Editor::ContentsWindow::SetDragDrop(GameObject* obj)
{
	if (UI::GetUI()->DragDropSource("DragDropContents", obj->GetName()))
	{
		m_objDragged = obj;
	}
	if (UI::GetUI()->DragDropTarget("DragDropContents"))
	{
		GameObject* ptr = obj->GetParent();

		while (ptr != nullptr)
		{
			if (ptr == m_objDragged)
			{
				return;
			}

			ptr = ptr->GetParent();
		}

		m_objDragged->SetParent(obj);
	}
}

void NIX::Editor::ContentsWindow::SwitchNewObj()
{
	switch (m_newObjType)
	{
	case OBJ_TYPE::EMPTY:
	{
		GameObject* empty = new GameObject();
		empty->SetParent(m_objRightClicked);
		std::vector<GameObject*>& ref = m_objRightClicked->GetChildren();
		ref.insert(ref.begin(), ref[ref.size() - 1]);
		ref.pop_back();

		break;
	}
	case OBJ_TYPE::DUPLICATE:
	{
		GameObject* duplicate = new GameObject(m_objRightClicked->GetParent(), *m_objRightClicked);
		m_objRightClicked->GetParent()->GetChildren().emplace_back(duplicate);
		std::vector<GameObject*>& ref = m_objRightClicked->GetParent()->GetChildren();
		ref.insert(std::find(ref.begin(), ref.end(), m_objRightClicked) + 1, ref[ref.size() - 1]);
		ref.pop_back();

		break;
	}
	case OBJ_TYPE::CUBE:
	{
		GameObject* cube = new GameObject("Cube");
		cube->SetParent(m_objRightClicked);
		std::vector<GameObject*>& ref = m_objRightClicked->GetChildren();
		ref.insert(ref.begin(), ref[ref.size() - 1]);
		ref.pop_back();

		cube->AddComponent(new Model(cube, "Cube"));
		cube->AddComponent(new CollisionBox(cube, cube->GetTransform()->GetWorldScale()));

		break;
	}
	case OBJ_TYPE::SPHERE:
	{
		GameObject* sphere = new GameObject("Sphere");
		sphere->SetParent(m_objRightClicked);
		std::vector<GameObject*>& ref = m_objRightClicked->GetChildren();
		ref.insert(ref.begin(), ref[ref.size() - 1]);
		ref.pop_back();

		sphere->AddComponent(new Model(sphere, "Sphere"));
		sphere->AddComponent(new CollisionSphere(sphere, 1.0f));

		break;
	}
	default:
		break;
	}

	m_newObjType = OBJ_TYPE::NONE;
}

void NIX::Editor::ContentsWindow::AssetsTreeClick(GameObject* obj)
{
	if (UI::GetUI()->IsTreeLabelClicked())
	{
		m_objSelected = obj;
		UIEditor::GetInspectorWindow().SetGameObject(obj);
	}
	if (UI::GetUI()->IsTreeLabelClicked(1))
	{
		m_objRightClicked = obj;
		m_isRightClicked = true;
	}
}

void NIX::Editor::ContentsWindow::UpdateContentsTree(GameObject* obj, std::string path)
{
	path += obj->GetName();
	int childnb = 0;

	for (NIX::Editor::GameObject* i : obj->GetChildren())
	{
		bool isSelected = (i == m_objSelected);
		bool isActive = i->IsActive();

		std::string absPath = path + std::to_string(childnb);

		if (!isActive) UI::GetUI()->PushStyleColor({.1f, .1f, .1f, 1.f});

		if (i->GetChildren().empty())
		{
			if (UI::GetUI()->AddLeaf(i->GetName() + "##" + absPath, isSelected))
			{
				if (!isActive) UI::GetUI()->PopStyleColor();
				AssetsTreeClick(i);
				UI::GetUI()->CloseNode();
			}
			SetDragDrop(i);
		}
		else
		{
			if (UI::GetUI()->AddNode(i->GetName() + "##" + absPath, isSelected, true))
			{
				if (!isActive) UI::GetUI()->PopStyleColor();
				SetDragDrop(i);
				AssetsTreeClick(i);
				UpdateContentsTree(i, path);
				UI::GetUI()->CloseNode();
			}
			else
			{
				if (!isActive) UI::GetUI()->PopStyleColor();
				SetDragDrop(i);
				AssetsTreeClick(i);
			}
		}

		childnb++;
	}
}

void NIX::Editor::ContentsWindow::UpdateContentsWindow()
{
	if (!UI::GetUI()->NewWindow("Contents", true) || !SceneManager::GetActiveScene()->sceneRoot)
	{
		UI::GetUI()->EndWindow();
		return;
	}

	if (UI::GetUI()->AddNode("[Root]", false, false, true))
	{
		if (UI::GetUI()->DragDropTarget("DragDropContents") && m_objDragged->GetParent() != SceneManager::GetActiveScene()->sceneRoot)
		{
			m_objDragged->SetParent(SceneManager::GetActiveScene()->sceneRoot);
		}

		if (UI::GetUI()->IsTreeLabelClicked(1))
		{
			m_objRightClicked = SceneManager::GetActiveScene()->sceneRoot;
			UI::GetUI()->OpenPopup("##RightClickedContentsTreeRoot");
			m_isRightClickedRoot = true;
		}

		UpdateContentsTree(SceneManager::GetActiveScene()->sceneRoot, "[Root]");

		SetPopup();

		SwitchNewObj();
		
		if (m_isDelete)
		{
			delete m_objRightClicked;
			UIEditor::GetInspectorWindow().SetGameObject(nullptr);
			m_isDelete = false;
		}

		UI::GetUI()->CloseNode();
	}

	UI::GetUI()->EndWindow();
}