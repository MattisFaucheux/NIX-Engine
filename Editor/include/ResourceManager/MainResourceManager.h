#pragma once

#include <string>

namespace NIX::Editor
{
	class TextureViewManager;
	class MeshManager;
	class MaterialManager;

	class MainResourceManager
	{
	private:
		inline static MainResourceManager* m_resourceManager = nullptr;

		TextureViewManager* m_textureViewManager = nullptr;
		MaterialManager* m_materialManager = nullptr;
		MeshManager* m_meshManager = nullptr;
		
	public:

		MainResourceManager();
		~MainResourceManager();
		
		[[nodiscard]] static MainResourceManager* GetResourceManager();
		[[nodiscard]] static TextureViewManager* GetTextureViewManager();
		[[nodiscard]] static MaterialManager* GetMaterialManager();
		[[nodiscard]] static MeshManager* GetMeshManager();

		[[nodiscard]] static void*& GetUiTextureId(const std::string& imagePath);
		[[nodiscard]] static void*& GetMaterialPreviewId(const std::string& materialPath);
	};
}