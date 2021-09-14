#pragma once

#include "Component.h"
#include "GameObject.h"

struct lua_State;

namespace NIX::Editor
{
	class Script : public Component
	{
	public:
		Script(GameObject* gameObject);
		Script(GameObject* gameObject, const Script& other);

		void Serialize(Core::SaveElement& parent) override;
		void Deserialize(const Core::SaveElement& data) override;

		void AddScript(std::string_view scriptPath);
		void RemoveScript(std::string_view scriptPath);

		[[nodiscard]] const std::vector<std::string>& GetScripts();

		void SetIsActive(bool value) override;

		void Start();
		void Update(float deltaTime);
		void OnTrigger(GameObject* other, bool triggerEnter);
		void OnContact(GameObject* other, bool contactBegin);

	private:

		std::vector<std::string> m_scriptPaths;
		std::vector<lua_State*> m_scriptLua;

	};
}