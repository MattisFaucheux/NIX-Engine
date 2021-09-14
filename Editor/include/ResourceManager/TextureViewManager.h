#pragma once

#include "Material/TextureView.h"
#include "ResourceManager/TResourceManager.h"

namespace NIX::Editor
{
	class TextureViewManager : public Core::TResourceManager<Rendering::TextureView*>
	{
	public :
		void Clear() override;

		void*& GetUITextureID(const std::string& imagePath);
		void UpdateTexturePtr(std::string_view path, Rendering::TextureView* newPtr);
	private:
		std::pair<bool, Rendering::TextureView*> LoadFile(const char* filename) override;
	};

}
