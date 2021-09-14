#include "UI/Components/UINodeCollisionConvex.h"
#include "Components/CollisionMesh.h"

#include "PhysicSimulation.h"
#include "UI/UIEditor.h"
#include "UI/UI.h"

namespace NIX::Editor
{
	void UINodeCollisionConvex::UpdateComponentProperties(GameObject* obj)
	{
		if (obj == nullptr)
		{
			return;
		}

		m_obj = obj;
		
		CollisionMesh* collisionMeshComp = m_obj->GetComponent<CollisionMesh>();
		if (collisionMeshComp == nullptr)
		{
			return;
		}
		
		
		m_isTrigger = collisionMeshComp->GetCollisionMesh()->GetIsTrigger();
		m_queryTestEnable = collisionMeshComp->GetCollisionMesh()->GetIsQueryTestEnable();
		m_isDebugDraw = collisionMeshComp->GetCollisionMesh()->IsDebugDraw();
	}

	void UINodeCollisionConvex::UpdateComponent()
	{
		CollisionMesh* collisionMeshComp = m_obj->GetComponent<CollisionMesh>();
		if (collisionMeshComp == nullptr)
		{
			return;
		}
		
		static bool isCollisionMeshOpen = true;

		UI::GetUI()->NewChild("Collision Convex", 0.f, isCollisionMeshOpen ? 107.f : 30.f, true);
		UI::GetUI()->SetNextItemOpen(isCollisionMeshOpen);

		if (UI::GetUI()->AddNode("Collision Convex"))
		{
			if (UIEditor::GetInspectorWindow().RightClickComp(collisionMeshComp))
			{
				UpdateComponentNode(isCollisionMeshOpen);
			}
			
			UI::GetUI()->CloseNode();
		}
		else
		{
			UIEditor::GetInspectorWindow().RightClickComp(collisionMeshComp);
			isCollisionMeshOpen = false;
		}

		UI::GetUI()->EndChild();
	}

	void UINodeCollisionConvex::UpdateComponentNode(bool& isComponentOpen)
	{
		CollisionMesh* collisionMeshComp = m_obj->GetComponent<CollisionMesh>();
		if (collisionMeshComp == nullptr)
		{
			return;
		}

		if (!UIEditor::GetInspectorWindow().OpenComponentNode(isComponentOpen, "##CollisionMeshTable"))
		{
			return;
		}

		UpdateComponentProperties(m_obj);

		
		UpdateIsTrigger(collisionMeshComp);
		UpdateQueryTestEnable(collisionMeshComp);
		UpdateIsDebugDraw(collisionMeshComp);

		UIEditor::GetInspectorWindow().CloseComponentNode();
	}

	void UINodeCollisionConvex::UpdateIsTrigger(CollisionMesh* collisionConvexComp)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Is Trigger");
		if (UI::GetUI()->AddCheckbox("##IsTriggerCollision", m_isTrigger))
		{
			collisionConvexComp->GetCollisionMesh()->SetIsTrigger(m_isTrigger);
		}
	}

	void UINodeCollisionConvex::UpdateQueryTestEnable(CollisionMesh* collisionConvexComp)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Query Test Enable");
		if (UI::GetUI()->AddCheckbox("##QueryTestCollision", m_queryTestEnable))
		{
			collisionConvexComp->GetCollisionMesh()->SetIsQueryTestEnable(m_queryTestEnable);
		}
	}

	void UINodeCollisionConvex::UpdateIsDebugDraw(CollisionMesh* collisionConvexComp)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Is Debug Draw");
		if (UI::GetUI()->AddCheckbox("##IsDebugDrawCollision", m_isDebugDraw))
		{
			collisionConvexComp->GetCollisionMesh()->IsDebugDraw(m_isDebugDraw);
			Physic::PhysicSimulation::GetSingleton()->UpdateSimulation(1.f / 60.f);
		}
	}
}
