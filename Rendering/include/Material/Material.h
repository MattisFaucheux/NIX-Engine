#pragma once

#include <string>

#include "API/DLLExportRendering.h"
#include "Descriptors/DescriptorSets.h"
#include "Descriptors/DescriptorSetLayout.h"
#include "Descriptors/DescriptorPool.h"


namespace NIX::Rendering
{
	class TextureView;

	struct Material
	{
		RENDERING_LIBRARY_API Material();
		Material(const Material& other) = delete;
		Material(Material&& other) noexcept = delete;
		Material& operator=(const Material& other) = delete;
		Material& operator=(Material&& other) noexcept = delete;
		~Material() = default;
		
		RENDERING_LIBRARY_API Material(TextureView* albedo, TextureView* normal = nullptr);

		RENDERING_LIBRARY_API [[nodiscard]] TextureView* CreateMaterialPreview();

		RENDERING_LIBRARY_API void UpdateAlbedoMap(TextureView* albedo);
		RENDERING_LIBRARY_API void UpdateNormalMap(TextureView* normal);
		
		TextureView* albedoMap = nullptr;
		TextureView* normalMap = nullptr;
		bool isNormalMap = false;

		DescriptorSets m_descriptorSets;
		DescriptorPool m_descriptorPool;
		DescriptorSetLayout m_descriptorSetLayout;

		std::string path = "";
	private :
		void InitializeDescriptorSets();

		
	};
}


