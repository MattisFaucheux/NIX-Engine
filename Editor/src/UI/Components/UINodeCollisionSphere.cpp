#include "UI/Components/UINodeCollisionSphere.h"


#include "PhysicSimulation.h"
#include "Components/CollisionSphere.h"
#include "UI/UIEditor.h"
#include "UI/UI.h"


namespace NIX::Editor
{
	void UINodeCollisionSphere::UpdateComponentProperties(GameObject* obj)
	{
		if (obj == nullptr)
		{
			return;
		}
		m_obj = obj;
		
		CollisionSphere* col = m_obj->GetComponent<CollisionSphere>();
		if (col == nullptr)
		{
			return;
		}
		
		m_radius = col->GetCollisionSphere()->GetRadius();
		m_isTrigger = col->GetCollisionSphere()->GetIsTrigger();
		m_queryTestEnable = col->GetCollisionSphere()->GetIsQueryTestEnable();
		m_isDebugDraw = col->GetCollisionSphere()->IsDebugDraw();
	}

	void UINodeCollisionSphere::UpdateComponent()
	{
		CollisionSphere* collisionSphereComp = m_obj->GetComponent<CollisionSphere>();
		if (collisionSphereComp == nullptr)
		{
			return;
		}
		
		static bool isCollisionSphereOpen = true;

		UI::GetUI()->NewChild("Collision Sphere", 0.f, isCollisionSphereOpen ? 130.f : 30.f, true);
		UI::GetUI()->SetNextItemOpen(isCollisionSphereOpen);

		if (UI::GetUI()->AddNode("Collision Sphere"))
		{
			if (UIEditor::GetInspectorWindow().RightClickComp(m_obj->GetComponent<CollisionSphere>()))
			{
				UpdateComponentNode(isCollisionSphereOpen);
			}
			UI::GetUI()->CloseNode();
		}
		else
		{
			UIEditor::GetInspectorWindow().RightClickComp(m_obj->GetComponent<CollisionSphere>());
			isCollisionSphereOpen = false;
		}

		UI::GetUI()->EndChild();
	}

	void UINodeCollisionSphere::UpdateComponentNode(bool& isComponentOpen)
	{
		CollisionSphere* collisionSphereComp = m_obj->GetComponent<CollisionSphere>();
		if (collisionSphereComp == nullptr)
		{
			return;
		}

		if (!UIEditor::GetInspectorWindow().OpenComponentNode(isComponentOpen, "##CollisionSphereTable"))
		{
			return;
		}
		
		UpdateComponentProperties(m_obj);

		UpdateRadius(collisionSphereComp);
		UpdateIsTrigger(collisionSphereComp);
		UpdateQueryTestEnable(collisionSphereComp);
		UpdateIsDebugDraw(collisionSphereComp);

		UIEditor::GetInspectorWindow().CloseComponentNode();
	}

	void UINodeCollisionSphere::UpdateRadius(CollisionSphere* collisionSphereComponent)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Radius");
		if (UI::GetUI()->AddFloat("##Radius", m_radius, true))
		{
			collisionSphereComponent->GetCollisionSphere()->SetRadius(m_radius);
		}
	}

	void UINodeCollisionSphere::UpdateIsTrigger(CollisionSphere* collisionSphereComponent)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Is Trigger");
		if (UI::GetUI()->AddCheckbox("##IsTriggerCollision", m_isTrigger))
		{
			collisionSphereComponent->GetCollisionSphere()->SetIsTrigger(m_isTrigger);
		}
	}

	void UINodeCollisionSphere::UpdateQueryTestEnable(CollisionSphere* collisionSphereComponent)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Query Test Enable");
		if (UI::GetUI()->AddCheckbox("##QueryTestCollision", m_queryTestEnable))
		{
			collisionSphereComponent->GetCollisionSphere()->SetIsQueryTestEnable(m_queryTestEnable);
		}
	}

	void UINodeCollisionSphere::UpdateIsDebugDraw(CollisionSphere* collisionSphereComponent)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Is Debug Draw");
		if (UI::GetUI()->AddCheckbox("##IsDebugDrawCollision", m_isDebugDraw))
		{
			collisionSphereComponent->GetCollisionSphere()->IsDebugDraw(m_isDebugDraw);
			Physic::PhysicSimulation::GetSingleton()->UpdateSimulation(1.f / 60.f);
		}
	}
}
