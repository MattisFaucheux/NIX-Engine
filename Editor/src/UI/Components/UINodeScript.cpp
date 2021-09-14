#include "UI/Components/UINodeScript.h"

#include "Components/Scripting/Script.h"
#include "UI/UIEditor.h"
#include "UI/UI.h"

namespace NIX::Editor
{
	void UINodeScript::UpdateComponentProperties(GameObject* obj)
	{
		if (obj == nullptr)
		{
			return;
		}

		m_obj = obj;
	}

	void UINodeScript::UpdateComponent()
	{
		if (m_obj == nullptr || m_obj->GetComponent<Script>() == nullptr)
		{
			return;
		}

		static bool isScriptOpen = true;

		UI::GetUI()->NewChild("Script", 0.0f, isScriptOpen ? 120.0f : 30.0f, true);
		UI::GetUI()->SetNextItemOpen(isScriptOpen);

		if (UI::GetUI()->AddNode("Script"))
		{
			if (UIEditor::GetInspectorWindow().RightClickComp(m_obj->GetComponent<Script>()))
				UpdateComponentNode(isScriptOpen);
			UI::GetUI()->CloseNode();
		}
		else
		{
			UIEditor::GetInspectorWindow().RightClickComp(m_obj->GetComponent<Script>());
			isScriptOpen = false;
		}

		UI::GetUI()->EndChild();
	}

	void UINodeScript::SetScriptDrag(std::filesystem::path path)
	{
		m_scriptDrag = path.string();
	}
	
	void UINodeScript::UpdateComponentNode(bool& isComponentOpen)
	{
		Script* scriptComp = m_obj->GetComponent<Script>();
		if (scriptComp == nullptr)
		{
			return;
		}

		if (!UIEditor::GetInspectorWindow().OpenComponentNode(isComponentOpen, "##ScriptTable"))
		{
			return;
		}
		
		UpdateComponentProperties(m_obj);

		UpdateScripts(scriptComp);
		
		UIEditor::GetInspectorWindow().CloseComponentNode();
	}

	void UINodeScript::UpdateScripts(Script* scriptComponent)
	{
		int nb = 0;
		for (std::string script : scriptComponent->GetScripts())
		{
			UIEditor::GetInspectorWindow().SetLeftText("Script " + std::to_string(nb));
			UI::GetUI()->AddInputText("##ScriptPath" + std::to_string(nb), "", script, true);
			if (UI::GetUI()->IsClicked(1))
			{
				UI::GetUI()->OpenPopup("RightClickedScript");
				m_rightClicked = script;
			}
			nb++;
		}

		if (UI::GetUI()->AddPopup("RightClickedScript", m_rightClick) == 0)
		{
			scriptComponent->RemoveScript(m_rightClicked);
		}

		UIEditor::GetInspectorWindow().SetLeftText("Add Script");
		std::string empty = "";
		UI::GetUI()->AddInputText("##ScriptPathToAdd", "Drop here", empty, true);
		if (UI::GetUI()->DragDropTarget("DragDropScripts"))
		{
			scriptComponent->AddScript(m_scriptDrag);
		}
	}
}
