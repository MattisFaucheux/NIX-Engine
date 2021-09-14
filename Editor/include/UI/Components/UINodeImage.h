#pragma once
#include <string>
#include <vector>

#include "UINodeHUD.h"

namespace std {
	namespace filesystem {
		class path;
	}
}

namespace NIX::Editor
{
	class Image;
	class GameObject;

	class UINodeImage : public UINodeHUD
	{
	private:		
		/// Properties

		std::string m_image = "";

		float m_sizeImage[2] = { 0.0f, 0.0f };

		std::string m_imagePath = "";
		std::string m_imageName = "";

		std::string m_imageDrag;
		std::string m_imageDragName;

		/// Right-Click

		std::vector<std::string> m_rightClick = { "Reset" };
		
	public:
		void UpdateComponentProperties(GameObject* obj) override;

		void UpdateComponent() override;

		void SetImageDrag(std::filesystem::path path);
	private:
		void UpdateComponentNode(bool& isComponentOpen) override;

		void UpdatePos(Image* imageComponent);
		void UpdateAnchors(Image* imageComponent);
		void UpdateZOrder(Image* imageComponent);
		void UpdateSize(Image* imageComponent);
		void UpdateImage(Image* imageComponent);
	};
}



