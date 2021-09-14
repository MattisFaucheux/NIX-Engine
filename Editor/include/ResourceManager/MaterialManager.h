#pragma once

#include "ResourceManager/TResourceManager.h"
#include "Material/Material.h"

#define FAILED_MAT_PATH "Resources\\Materials\\failedMaterial.mat"
#define FAILED_TEXTURE_PATH "Resources\\Textures\\failedTexture.jpg"

namespace NIX {
	namespace Core {
		struct ParseMaterial;
	}
}

namespace NIX::Editor
{
	class MaterialManager : public Core::TResourceManager<Rendering::Material*>
	{
	public:
		bool AddResource(Rendering::Material* resource, const char* filename) override;

		void Clear() override;

		void LoadAllMaterials();
	private:
		std::pair<bool, Rendering::Material*> LoadFile(const char* filename) override;

		bool LoadFailedMaterial(Core::ParseMaterial& parseMaterial);

	};
}
