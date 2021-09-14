#include "UI/Components/UINodeRigidStatic.h"
#include "UI/UIEditor.h"
#include "UI/UI.h"
#include "Components/RigidStatic.h"

namespace NIX::Editor
{
	void UINodeRigidStatic::UpdateComponentProperties(GameObject* obj)
	{
		if (obj == nullptr)
		{
			return;
		}

		m_obj = obj;
	}

	void UINodeRigidStatic::UpdateComponent()
	{
		RigidStatic* rigidStaticComp = m_obj->GetComponent<RigidStatic>();
		if (rigidStaticComp == nullptr)
		{
			return;
		}
		
		UI::GetUI()->NewChild("RigidStatic", 0.f, 30.f, true);

		UI::GetUI()->AddText("   RigidStatic");
		
		UIEditor::GetInspectorWindow().RightClickComp(rigidStaticComp);

		UI::GetUI()->EndChild();
	}

	void UINodeRigidStatic::UpdateComponentNode(bool& /*isComponentOpen*/)
	{
		// Need to be override but not used
	}
}
