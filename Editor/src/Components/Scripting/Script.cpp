#include "Components/Scripting/Script.h"

#include "Scripting/ScriptManager.h"

namespace NIX::Editor
{
	Script::Script(GameObject* gameObject):
		Component(gameObject)
	{}

	Script::Script(GameObject* gameObject, const Script& other) :
		Component(gameObject)
	{
		for (int i = 0; i < (int)other.m_scriptPaths.size(); i++)
		{
			AddScript(other.m_scriptPaths[i]);
		}

		SetIsActive(gameObject->IsActive());
	}

	void Script::Serialize(Core::SaveElement& parent)
	{
		Core::SaveElement scriptsElem(parent, "scripts");

		scriptsElem.AddIntAttribute("scripts_nb", (int)m_scriptPaths.size());

		for (auto& scriptPath : m_scriptPaths)
		{
			Core::SaveElement script(scriptsElem, "script");
			script.AddStringAttribute("script_path", scriptPath.c_str());
		}
	}

	void Script::Deserialize(const Core::SaveElement& data)
	{
		std::pair<bool, Core::SaveElement> scriptElem = data.GetFirstChild();
		
		while (scriptElem.first)
		{
			std::string scriptPath = scriptElem.second.GetStringAttribute("script_path");
			AddScript(scriptPath);
			scriptElem = scriptElem.second.GetNextSibling();
		}
	}

	void Script::AddScript(std::string_view scriptPath)
	{
		if (!scriptPath.empty())
		{
			m_scriptPaths.emplace_back(scriptPath.data());
			m_scriptLua.push_back(ScriptManager::GetSingleton()->CreateNewState(scriptPath));
		}
	}

	void Script::RemoveScript(std::string_view scriptPath)
	{
		for (size_t i = 0; i < m_scriptPaths.size(); i++)
		{
			if (m_scriptPaths[i] == scriptPath)
			{
				m_scriptPaths.erase(m_scriptPaths.begin() + i);
				m_scriptLua.erase(m_scriptLua.begin() + i);
				break;
			}
		}
	}

	const std::vector<std::string>& Script::GetScripts()
	{
		return m_scriptPaths;
	}

	void Script::SetIsActive(bool value)
	{
		m_isActive = value;
	}

	void Script::Start()
	{
		for (int i = 0; i < (int)m_scriptPaths.size(); i++)
		{
			ScriptManager::GetSingleton()->CallFunc(m_scriptLua[i], "Start", m_gameObject);
		}
	}

	void Script::Update(float deltaTime)
	{
		for (int i = 0; i < (int)m_scriptPaths.size(); i++)
		{			
			ScriptManager::GetSingleton()->CallFunc(m_scriptLua[i], "Update", m_gameObject, deltaTime);
		}
	}

	void Script::OnTrigger(GameObject* other, bool triggerEnter)
	{
		for (int i = 0; i < (int)m_scriptPaths.size(); i++)
		{
			if (triggerEnter)
			{
				ScriptManager::GetSingleton()->CallFunc(m_scriptLua[i], "OnTriggerEnter", m_gameObject, other);
			}
			else
			{
				ScriptManager::GetSingleton()->CallFunc(m_scriptLua[i], "OnTriggerExit", m_gameObject, other);
			}
		}
	}

	void Script::OnContact(GameObject* other, bool contactBegin)
	{
		for (int i = 0; i < (int)m_scriptPaths.size(); i++)
		{
			if (contactBegin)
			{
				ScriptManager::GetSingleton()->CallFunc(m_scriptLua[i], "OnCollisionEnter", m_gameObject, other);
			}
			else
			{
				ScriptManager::GetSingleton()->CallFunc(m_scriptLua[i], "OnCollisionExit", m_gameObject, other);
			}
		}
	}
}