#pragma once



namespace NIX::Editor
{
	class GameObject;
	
	class UINodeComponent
	{
	protected:
		GameObject* m_obj = nullptr;
		
	public:
		virtual ~UINodeComponent() {}
		
		virtual void UpdateComponentProperties(GameObject* obj) = 0;

		virtual void UpdateComponent() = 0;
		
	protected:
		virtual void UpdateComponentNode(bool& isComponentOpen) = 0;
		
	};
}
