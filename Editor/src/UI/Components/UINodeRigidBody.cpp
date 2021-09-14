#include "UI/Components/UINodeRigidBody.h"

#include <iomanip>
#include <sstream>

#include "Components/RigidBody.h"
#include "RigidBody.h"
#include "UI/UIEditor.h"
#include "UI/UI.h"

namespace NIX::Editor
{
	void UINodeRigidBody::UpdateComponentProperties(GameObject* obj)
	{
		if (obj == nullptr)
		{
			return;
		}

		m_obj = obj;
		
		RigidBody* rigidBodyComp = m_obj->GetComponent<RigidBody>();
		if (rigidBodyComp == nullptr)
		{
			return;
		}

		m_mass = rigidBodyComp->GetRigidBody()->GetMass();
		m_isGravity = rigidBodyComp->GetRigidBody()->GetIsGravity();
		m_kinematic = rigidBodyComp->GetRigidBody()->GetIsKinematic();

		m_freezePosX = rigidBodyComp->GetRigidBody()->GetIsXTranslationLock();
		m_freezePosY = rigidBodyComp->GetRigidBody()->GetIsYTranslationLock();
		m_freezePosZ = rigidBodyComp->GetRigidBody()->GetIsZTranslationLock();

		m_freezeRotX = rigidBodyComp->GetRigidBody()->GetIsXRotationLock();
		m_freezeRotY = rigidBodyComp->GetRigidBody()->GetIsYRotationLock();
		m_freezeRotZ = rigidBodyComp->GetRigidBody()->GetIsZRotationLock();

		m_linearDamping = rigidBodyComp->GetRigidBody()->GetLinearDamping();
		m_angularDamping = rigidBodyComp->GetRigidBody()->GetAngularDamping();
		m_maxLinearVelocity = rigidBodyComp->GetRigidBody()->GetMaxLinearVelocity();
		m_maxAngularVelocity = rigidBodyComp->GetRigidBody()->GetMaxAngularVelocity();

		const LibMath::Vector3 massInertia = rigidBodyComp->GetRigidBody()->GetMassInertiaTensor();

		m_massInertiaTensor[0] = massInertia.x;
		m_massInertiaTensor[1] = massInertia.y;
		m_massInertiaTensor[2] = massInertia.z;

		const LibMath::Vector3 centerMass = rigidBodyComp->GetRigidBody()->GetLocalCenterOfMass();

		m_centerOfMass[0] = centerMass.x;
		m_centerOfMass[1] = centerMass.y;
		m_centerOfMass[2] = centerMass.z;
	}

	void UINodeRigidBody::UpdateComponent()
	{
		RigidBody* rigidBodyComp = m_obj->GetComponent<RigidBody>();
		if (rigidBodyComp == nullptr)
		{
			return;
		}
		
		static bool isRigidBodyOpen = true;

		UI::GetUI()->NewChild("RigidBody", 0.f, isRigidBodyOpen ? (m_isConstraintsOpen ? 320.f : 265.f) : 30.f, true);
		UI::GetUI()->SetNextItemOpen(isRigidBodyOpen);

		if (UI::GetUI()->AddNode("RigidBody"))
		{
			if (UIEditor::GetInspectorWindow().RightClickComp(rigidBodyComp))
			{
				UpdateComponentNode(isRigidBodyOpen);
			}
				
			UI::GetUI()->CloseNode();
		}
		else
		{
			UIEditor::GetInspectorWindow().RightClickComp(rigidBodyComp);
			isRigidBodyOpen = false;
		}

		UI::GetUI()->EndChild();
	}

	void UINodeRigidBody::UpdateComponentNode(bool& isComponentOpen)
	{
		RigidBody* rigidBodyComp = m_obj->GetComponent<RigidBody>();
		if (rigidBodyComp == nullptr)
		{
			return;
		}
		
		if (!UIEditor::GetInspectorWindow().OpenComponentNode(isComponentOpen, "##RigidBodyTable"))
		{
			return;
		}
		
		UpdateComponentProperties(m_obj);

		UpdateMass(rigidBodyComp);
		UpdateIsGravity(rigidBodyComp);
		UpdateIsKinematic(rigidBodyComp);
		UpdateConstraints(rigidBodyComp);
		UpdateLinearDamping(rigidBodyComp);
		UpdateAngularDamping(rigidBodyComp);
		UpdateMaxLinearVelocity(rigidBodyComp);
		UpdateMaxAngularVelocity(rigidBodyComp);
		UpdateMaxInertiaTensor(rigidBodyComp);
		UpdateLocalCenterOfMass();
			
		UIEditor::GetInspectorWindow().CloseComponentNode();
		
	}

	void UINodeRigidBody::UpdateMass(RigidBody* rigidBodyComponent)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Mass");
		if (UI::GetUI()->AddFloat("##Mass", m_mass, true))
		{
			rigidBodyComponent->GetRigidBody()->SetMass(m_mass);
		}
	}

	void UINodeRigidBody::UpdateIsGravity(RigidBody* rigidBodyComponent)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Use Gravity");
		if (UI::GetUI()->AddCheckbox("##UseGravity", m_isGravity))
		{
			rigidBodyComponent->GetRigidBody()->SetIsGravity(m_isGravity);
		}
	}

	void UINodeRigidBody::UpdateIsKinematic(RigidBody* rigidBodyComponent)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Is Kinematic");
		if (UI::GetUI()->AddCheckbox("##IsKinematic", m_kinematic))
		{
			rigidBodyComponent->GetRigidBody()->SetIsKinematic(m_kinematic);
		}
	}

	void UINodeRigidBody::UpdateConstraints(RigidBody* rigidBodyComponent)
	{
		UI::GetUI()->TableNextRow();
		UI::GetUI()->TableSetColumnIndex(0);
		if (UI::GetUI()->AddNode("Constraints"))
		{
			UI::GetUI()->SetCursorPosY(UI::GetUI()->GetCursorPosY() + 3.f);
			UpdateConstraintsNode(rigidBodyComponent);
			UI::GetUI()->CloseNode();
		}
		else
		{
			UI::GetUI()->SetCursorPosY(UI::GetUI()->GetCursorPosY() + 3.f);
			m_isConstraintsOpen = false;
		}
	}

	void UINodeRigidBody::UpdateLinearDamping(RigidBody* rigidBodyComponent)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Linear Damping");
		if (UI::GetUI()->AddFloat("##LinearDamping", m_linearDamping, true))
		{
			rigidBodyComponent->GetRigidBody()->SetLinearDamping(m_linearDamping);
		}
	}

	void UINodeRigidBody::UpdateAngularDamping(RigidBody* rigidBodyComponent)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Angular Damping");
		if (UI::GetUI()->AddFloat("##AngularDamping", m_angularDamping, true))
		{
			rigidBodyComponent->GetRigidBody()->SetAngularDamping(m_angularDamping);
		}
	}

	void UINodeRigidBody::UpdateMaxLinearVelocity(RigidBody* rigidBodyComponent)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Max Linear Velocity");
		if (UI::GetUI()->AddFloat("##MaxLinearVelocity", m_maxLinearVelocity, true))
		{
			rigidBodyComponent->GetRigidBody()->SetMaxLinearVelocity(m_maxLinearVelocity);
		}
	}

	void UINodeRigidBody::UpdateMaxAngularVelocity(RigidBody* rigidBodyComponent)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Max Angular Velocity");
		if (UI::GetUI()->AddFloat("##MaxAngularVelocity", m_maxAngularVelocity, true))
		{
			rigidBodyComponent->GetRigidBody()->SetMaxAngularVelocity(m_maxAngularVelocity);
		}
	}

	void UINodeRigidBody::UpdateMaxInertiaTensor(RigidBody* rigidBodyComponent)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Mass Inertia Tensor");
		if (UI::GetUI()->AddFloat3("##MassInertiaTensor", m_massInertiaTensor, true))
		{
			rigidBodyComponent->GetRigidBody()->SetMassInertiaTensor({ m_massInertiaTensor[0], m_massInertiaTensor[1], m_massInertiaTensor[2] });
		}
	}

	void UINodeRigidBody::UpdateLocalCenterOfMass()
	{
		UIEditor::GetInspectorWindow().SetLeftText("Local Center Of Mass");
		if (UI::GetUI()->AddTable("##RigidBodyTable", 3))
		{
			std::stringstream ss1, ss2, ss3;
			ss1 << std::fixed << std::setprecision(4) << m_centerOfMass[0];
			ss2 << std::fixed << std::setprecision(4) << m_centerOfMass[1];
			ss3 << std::fixed << std::setprecision(4) << m_centerOfMass[2];

			UI::GetUI()->TableNextColumn();
			UI::GetUI()->AddTextDisabled(ss1.str());
			UI::GetUI()->TableNextColumn();
			UI::GetUI()->AddTextDisabled(ss2.str());
			UI::GetUI()->TableNextColumn();
			UI::GetUI()->AddTextDisabled(ss3.str());
			UI::GetUI()->CloseTable();
		}
	}

	void UINodeRigidBody::UpdateConstraintsNode(RigidBody* rigidBodyComponent)
	{
		if (!m_isConstraintsOpen)
		{
			m_isConstraintsOpen = true;
			return;
		}

		UpdateFreezePositionNode(rigidBodyComponent);
		UpdateFreezeRotationNode(rigidBodyComponent);
	}

	void UINodeRigidBody::UpdateFreezePositionNode(RigidBody* rigidBodyComponent)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Freeze Position");

		if (UI::GetUI()->AddTable("##RBFreezePosTable", 3))
		{
			UI::GetUI()->TableNextColumn();
			if (UI::GetUI()->AddCheckbox("##FreezePosX", m_freezePosX))
			{
				rigidBodyComponent->GetRigidBody()->SetIsXTranslationLock(m_freezePosX);
			}
			SetTextConstraints("X");
			if (UI::GetUI()->AddCheckbox("##FreezePosY", m_freezePosY))
			{
				rigidBodyComponent->GetRigidBody()->SetIsYTranslationLock(m_freezePosY);
			}
			SetTextConstraints("Y");
			if (UI::GetUI()->AddCheckbox("##FreezePosZ", m_freezePosZ))
			{
				rigidBodyComponent->GetRigidBody()->SetIsZTranslationLock(m_freezePosZ);
			}
			UI::GetUI()->SameLine();
			UI::GetUI()->AddText("Z");
			UI::GetUI()->CloseTable();
		}
	}

	void UINodeRigidBody::UpdateFreezeRotationNode(RigidBody* rigidBodyComponent)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Freeze Rotation");
		
		if (UI::GetUI()->AddTable("##RBFreezeRotTable", 3))
		{
			UI::GetUI()->TableNextColumn();
			if (UI::GetUI()->AddCheckbox("##FreezeRotX", m_freezeRotX))
			{
				rigidBodyComponent->GetRigidBody()->SetIsXRotationLock(m_freezeRotX);
			}
			SetTextConstraints("X");
			if (UI::GetUI()->AddCheckbox("##FreezeRotY", m_freezeRotY))
			{
				rigidBodyComponent->GetRigidBody()->SetIsYRotationLock(m_freezeRotY);
			}
			SetTextConstraints("Y");
			if (UI::GetUI()->AddCheckbox("##FreezeRotZ", m_freezeRotZ))
			{
				rigidBodyComponent->GetRigidBody()->SetIsZRotationLock(m_freezeRotZ);
			}
			UI::GetUI()->SameLine();
			UI::GetUI()->AddText("Z");
			UI::GetUI()->CloseTable();
		}
	}

	void UINodeRigidBody::SetTextConstraints(std::string_view text)
	{
		UI::GetUI()->SameLine();
		UI::GetUI()->AddText(text.data());
		UI::GetUI()->TableNextColumn();
	}
}
