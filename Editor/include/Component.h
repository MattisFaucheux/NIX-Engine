#pragma once

#include "Serialization/SceneSave/Serializer.h"

namespace NIX::Editor
{	
	class GameObject;
	
	class Component
	{		
	public:

		Component(GameObject* parent);
		
		virtual void Serialize(Core::SaveElement& parent) = 0;
		virtual void Deserialize(const Core::SaveElement& data) = 0;

		virtual ~Component();

		[[nodiscard]] bool GetIsActive() const;
		virtual void SetIsActive(bool value) = 0;

		[[nodiscard]] GameObject* GetParent() const;

	protected:
		bool m_isActive = true;

		GameObject* m_gameObject;
	};
}
