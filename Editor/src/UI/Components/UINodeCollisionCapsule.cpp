#include "UI/Components/UINodeCollisionCapsule.h"


#include "PhysicSimulation.h"
#include "UI/UIEditor.h"
#include "UI/UI.h"
#include "Components/CollisionCapsule.h"

namespace NIX::Editor
{
	void UINodeCollisionCapsule::UpdateComponentProperties(GameObject* obj)
	{
		if (obj == nullptr)
		{ 
			return;
		}

		m_obj = obj;

		CollisionCapsule* collisionCapsuleComp = m_obj->GetComponent<CollisionCapsule>();
		if (collisionCapsuleComp == nullptr)
		{
			return;
		}
		
		m_radius = collisionCapsuleComp->GetCollisionCapsule()->GetRadius();
		m_height = collisionCapsuleComp->GetCollisionCapsule()->GetHeight();
		m_isTrigger = collisionCapsuleComp->GetCollisionCapsule()->GetIsTrigger();
		m_queryTestEnable = collisionCapsuleComp->GetCollisionCapsule()->GetIsQueryTestEnable();
		m_isDebugDraw = collisionCapsuleComp->GetCollisionCapsule()->IsDebugDraw();
	}

	void UINodeCollisionCapsule::UpdateComponent()
	{
		CollisionCapsule* collisionCapsuleComp = m_obj->GetComponent<CollisionCapsule>();
		if (collisionCapsuleComp == nullptr)
		{
			return;
		}
		
		static bool isCollisionCapsuleOpen = true;

		UI::GetUI()->NewChild("Collision Capsule", 0.f, isCollisionCapsuleOpen ? 155.f : 30.f, true);
		UI::GetUI()->SetNextItemOpen(isCollisionCapsuleOpen);

		if (UI::GetUI()->AddNode("Collision Capsule"))
		{
			if (UIEditor::GetInspectorWindow().RightClickComp(collisionCapsuleComp))
			{
				UpdateComponentNode(isCollisionCapsuleOpen);
			}
			
			UI::GetUI()->CloseNode();
		}
		else
		{
			UIEditor::GetInspectorWindow().RightClickComp(collisionCapsuleComp);
			isCollisionCapsuleOpen = false;
		}

		UI::GetUI()->EndChild();
	}

	void UINodeCollisionCapsule::UpdateComponentNode(bool& isComponentOpen)
	{
		CollisionCapsule* collisionCapsuleComp = m_obj->GetComponent<CollisionCapsule>();
		if (collisionCapsuleComp == nullptr)
		{
			return;
		}

		if (!UIEditor::GetInspectorWindow().OpenComponentNode(isComponentOpen, "##CollisionCapsuleTable"))
		{
			return;
		}

		UpdateComponentProperties(m_obj);

		UpdateRadius(collisionCapsuleComp);
		UpdateHeight(collisionCapsuleComp);
		UpdateIsTrigger(collisionCapsuleComp);
		UpdateQueryTestEnable(collisionCapsuleComp);
		UpdateIsDebugDraw(collisionCapsuleComp);

		UIEditor::GetInspectorWindow().CloseComponentNode();
	}

	void UINodeCollisionCapsule::UpdateRadius(CollisionCapsule* collisionCapsuleComponent)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Radius");
		if (UI::GetUI()->AddFloat("##Radius", m_radius, true))
		{
			collisionCapsuleComponent->GetCollisionCapsule()->SetRadius(m_radius);
		}
	}

	void UINodeCollisionCapsule::UpdateHeight(CollisionCapsule* collisionCapsuleComponent)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Height");
		if (UI::GetUI()->AddFloat("##Height", m_height, true))
		{
			collisionCapsuleComponent->GetCollisionCapsule()->SetHeight(m_height);
		}
	}

	void UINodeCollisionCapsule::UpdateIsTrigger(CollisionCapsule* collisionCapsuleComponent)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Is Trigger");
		if (UI::GetUI()->AddCheckbox("##IsTriggerCollision", m_isTrigger))
		{
			collisionCapsuleComponent->GetCollisionCapsule()->SetIsTrigger(m_isTrigger);
		}
	}

	void UINodeCollisionCapsule::UpdateQueryTestEnable(CollisionCapsule* collisionCapsuleComponent)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Query Test Enable");
		if (UI::GetUI()->AddCheckbox("##QueryTestCollision", m_queryTestEnable))
		{
			collisionCapsuleComponent->GetCollisionCapsule()->SetIsQueryTestEnable(m_queryTestEnable);
		}
	}

	void UINodeCollisionCapsule::UpdateIsDebugDraw(CollisionCapsule* collisionCapsuleComponent)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Is Debug Draw");
		if (UI::GetUI()->AddCheckbox("##IsDebugDrawCollision", m_isDebugDraw))
		{
			collisionCapsuleComponent->GetCollisionCapsule()->IsDebugDraw(m_isDebugDraw);
			Physic::PhysicSimulation::GetSingleton()->UpdateSimulation(1.f / 60.f);
		}
	}
}
