#include "Component.h"

#include "Log/Logger.h"
#include "GameObject.h"

#include "Scripting/ScriptManager.h"

namespace NIX::Editor
{
	Component::Component(GameObject* parent)
		: m_gameObject(parent)
	{
		if (parent == nullptr)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_WARNING, "Parent gameobject of the compoenent is null");
		}
		
	}
	
	Component::~Component()
	{
	}

	bool Component::GetIsActive() const
	{
		return m_isActive;
	}
	GameObject* Component::GetParent() const
	{
		return m_gameObject;
	}
}


