#include "UI/Components/UINodeCollisionBox.h"

#include "Components/CollisionBox.h"
#include "CollisionBox.h"
#include "PhysicSimulation.h"
#include "UI/UIEditor.h"
#include "UI/UI.h"

namespace NIX::Editor
{
	void UINodeCollisionBox::UpdateComponentProperties(GameObject* obj)
	{
		if (obj == nullptr)
		{
			return;
		}

		m_obj = obj;
		
		CollisionBox* col = m_obj->GetComponent<CollisionBox>();
		if (col == nullptr)
		{
			return;
		}
		
		const LibMath::Vector3 scale = col->GetCollisionBox()->GetScale();

		m_scale[0] = scale.x;
		m_scale[1] = scale.y;
		m_scale[2] = scale.z;

		m_isTrigger = col->GetCollisionBox()->GetIsTrigger();
		m_queryTestEnable = col->GetCollisionBox()->GetIsQueryTestEnable();
		m_isDebugDraw = col->GetCollisionBox()->IsDebugDraw();
	}

	void UINodeCollisionBox::UpdateComponent()
	{
		CollisionBox* collisionBoxComp = m_obj->GetComponent<CollisionBox>();
		if (collisionBoxComp == nullptr)
		{
			return;
		}
		
		static bool isCollisionBoxOpen = true;

		UI::GetUI()->NewChild("Collision Box", 0.f, isCollisionBoxOpen ? 130.f : 30.f, true);
		UI::GetUI()->SetNextItemOpen(isCollisionBoxOpen);

		if (UI::GetUI()->AddNode("Collision Box"))
		{
			if (UIEditor::GetInspectorWindow().RightClickComp(m_obj->GetComponent<CollisionBox>()))
			{
				UpdateComponentNode(isCollisionBoxOpen);
			}
			
			UI::GetUI()->CloseNode();
		}
		else
		{
			UIEditor::GetInspectorWindow().RightClickComp(m_obj->GetComponent<CollisionBox>());
			isCollisionBoxOpen = false;
		}

		UI::GetUI()->EndChild();
	}

	void UINodeCollisionBox::UpdateComponentNode(bool& isComponentOpen)
	{
		CollisionBox* collisionBoxComp = m_obj->GetComponent<CollisionBox>();
		if (collisionBoxComp == nullptr)
		{
			return;
		}

		if (!UIEditor::GetInspectorWindow().OpenComponentNode(isComponentOpen, "##CollisionBoxTable"))
		{
			return;
		}
		
		UpdateComponentProperties(m_obj);

		UpdateScale(collisionBoxComp);
		UpdateIsTrigger(collisionBoxComp);
		UpdateQueryTestEnable(collisionBoxComp);
		UpdateIsDebugDraw(collisionBoxComp);
		
		UIEditor::GetInspectorWindow().CloseComponentNode();
	}

	void UINodeCollisionBox::UpdateScale(CollisionBox* collisionBoxComponent)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Scale");
		if (UI::GetUI()->AddFloat3("##ScaleCollision", m_scale, true))
		{
			collisionBoxComponent->GetCollisionBox()->SetScale({ m_scale[0], m_scale[1], m_scale[2] });
		}		
	}

	void UINodeCollisionBox::UpdateIsTrigger(CollisionBox* collisionBoxComponent)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Is Trigger");
		if (UI::GetUI()->AddCheckbox("##IsTriggerCollision", m_isTrigger))
		{
			collisionBoxComponent->GetCollisionBox()->SetIsTrigger(m_isTrigger);
		}
	}

	void UINodeCollisionBox::UpdateQueryTestEnable(CollisionBox* collisionBoxComponent)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Query Test Enable");
		if (UI::GetUI()->AddCheckbox("##QueryTestCollision", m_queryTestEnable))
		{
			collisionBoxComponent->GetCollisionBox()->SetIsQueryTestEnable(m_queryTestEnable);
		}
	}

	void UINodeCollisionBox::UpdateIsDebugDraw(CollisionBox* collisionBoxComponent)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Is Debug Draw");
		if (UI::GetUI()->AddCheckbox("##IsDebugDrawCollision", m_isDebugDraw))
		{
			collisionBoxComponent->GetCollisionBox()->IsDebugDraw(m_isDebugDraw);
			Physic::PhysicSimulation::GetSingleton()->UpdateSimulation(1.f / 60.f);
		}
	}
}
