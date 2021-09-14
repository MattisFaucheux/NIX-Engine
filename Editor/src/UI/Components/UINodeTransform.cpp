#include "UI/Components/UINodeTransform.h"

#include <Quaternion/Quaternion.h>


#include "UI/UI.h"
#include "GameObject.h"
#include "UI/InspectorWindow.h"
#include "UI/UIEditor.h"

namespace NIX::Editor
{
	void UINodeTransform::UpdateComponentProperties(GameObject* obj)
	{
		if (obj == nullptr)
		{
			return;
		}

		m_obj = obj;

		const LibMath::Vector3 pos = m_obj->GetTransform()->GetLocalPosition();
		const LibMath::Quaternion rot = m_obj->GetTransform()->GetLocalRotation();
		const LibMath::Vector3 scale = m_obj->GetTransform()->GetLocalScale();

		m_vecPosition[0] = pos.x;
		m_vecPosition[1] = pos.y;
		m_vecPosition[2] = pos.z;

		if (rot != LibMath::Quaternion(LibMath::Rad(LibMath::Deg(m_vecRotation[0])),
			LibMath::Rad(LibMath::Deg(m_vecRotation[1])),
			LibMath::Rad(LibMath::Deg(m_vecRotation[2]))))
		{
			LibMath::Vector3 rotEuler = rot.EulerAngles();
			m_vecRotation[0] = rotEuler.x;
			m_vecRotation[1] = rotEuler.y;
			m_vecRotation[2] = rotEuler.z;
		}

		m_vecScale[0] = scale.x;
		m_vecScale[1] = scale.y;
		m_vecScale[2] = scale.z;
	}

	void UINodeTransform::UpdateComponent()
	{
		static bool isTransformOpen = true;

		UI::GetUI()->NewChild("Transform", 0.f, isTransformOpen ? 105.f : 30.f, true);
		UI::GetUI()->SetNextItemOpen(isTransformOpen);

		if (UI::GetUI()->AddNode("Transform"))
		{
			RightClick();
			UpdateComponentNode(isTransformOpen);
			UI::GetUI()->CloseNode();
		}
		else
		{
			isTransformOpen = false;
		}

		UI::GetUI()->EndChild();
	}

	void UINodeTransform::UpdateComponentNode(bool& isComponentOpen)
	{
		Transform* transformComp = m_obj->GetTransform();
		if (transformComp == nullptr)
		{
			return;
		}
		

		if (!UIEditor::GetInspectorWindow().OpenComponentNode(isComponentOpen, "##TransformTable"))
		{
			return;
		}

		UpdateComponentProperties(m_obj);

		UpdatePosition(transformComp);
		UpdateRotation(transformComp);
		UpdateScale(transformComp);		

		UIEditor::GetInspectorWindow().CloseComponentNode();
	}

	void UINodeTransform::UpdatePosition(Transform* transformComponent)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Position");
		if (UI::GetUI()->AddFloat3("##PositionTransform", m_vecPosition))
		{
			transformComponent->SetLocalPosition({ m_vecPosition[0], m_vecPosition[1], m_vecPosition[2] });
		}
	}

	void UINodeTransform::UpdateRotation(Transform* transformComponent)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Rotation");
		if (UI::GetUI()->AddFloat3("##RotationTransform", m_vecRotation))
		{
			transformComponent->SetLocalRotation({ LibMath::Rad(LibMath::Deg(m_vecRotation[0])),
														LibMath::Rad(LibMath::Deg(m_vecRotation[1])),
														LibMath::Rad(LibMath::Deg(m_vecRotation[2])) });
		}
	}

	void UINodeTransform::UpdateScale(Transform* transformComponent)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Scale");
		if (UI::GetUI()->AddFloat3("##ScaleTransform", m_vecScale))
		{
			transformComponent->SetLocalScale({ m_vecScale[0], m_vecScale[1], m_vecScale[2] });
		}
	}

	void UINodeTransform::RightClick()
	{
		if (UI::GetUI()->IsClicked(1))
		{
			UI::GetUI()->OpenPopup("##RightClickTransform");
		}
		if (UI::GetUI()->AddPopup("##RightClickTransform", m_rightClick) == 0)
		{
			Transform* trans = m_obj->GetTransform();

			trans->SetLocal(LibMath::Vector3::Zero, LibMath::Quaternion::Quaternion(), LibMath::Vector3::One);
		}
	}
}
