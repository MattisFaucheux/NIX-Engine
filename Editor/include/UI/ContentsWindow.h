#pragma once

#include <vector>
#include <string>

namespace NIX::Editor
{
	class GameObject;

	class ContentsWindow
	{
		enum class OBJ_TYPE
		{
			NONE = -1,
			EMPTY,
			DUPLICATE,
			CUBE,
			SPHERE
		};

		GameObject* m_objSelected;
		GameObject* m_objDragged;
		GameObject* m_objRightClicked;

		bool m_isDelete = false;

		OBJ_TYPE m_newObjType = OBJ_TYPE::NONE;

		std::vector<std::string> m_rightClickRoot = { "Add Empty", "Add Cube", "Add Sphere" };
		std::vector<std::string> m_rightClick = { "Duplicate", "Delete", "##Separator", "Add Empty", "Add Cube", "Add Sphere" };

		bool m_isRightClickedRoot = false;
		bool m_isRightClicked = false;
		bool m_isRightClickedOnce = true;

		void SetPopup();
		void SetDragDrop(GameObject* obj);
		void SwitchNewObj();
		void AssetsTreeClick(GameObject* obj);
		void UpdateContentsTree(GameObject* obj, std::string path);

	public:
		void UpdateContentsWindow();
	};

}
