#pragma once
#include <string>
#include <filesystem>

#include "UINodeComponent.h"

namespace NIX::Editor
{
	class Model;
	class GameObject;

	class UINodeModel : public UINodeComponent
	{
	private:		
		/// Right-Click

		std::vector<std::string> m_rightClickMat = { "Reset" };
		std::vector<std::string> m_rightClickMesh = { "Set Cube", "Set Sphere" };

		///
		std::string m_meshPath = "";
		std::string m_materialPath = "";
		std::string m_materialName = "";

		std::string m_meshDrag;

		std::string m_materialDrag;
		std::string m_materialDragName;

	public:		
		void UpdateComponentProperties(GameObject* obj) override;
		void UpdateComponent() override;

		void SetMeshDrag(std::filesystem::path path);
		void SetMaterialDrag(std::filesystem::path path);
		
	private:
		void UpdateComponentNode(bool& isComponentOpen) override;
		void UpdateMesh(Model* modelComponent);
		void UpdateMaterial(Model* modelComponent);
	};
}
