#pragma once

#include "API/DLLExportRendering.h"

#include <vector>

constexpr int NB_LIGHTS_MAX = 250; // Need to be the same inside the frag shader used

namespace NIX::Rendering
{
	enum class ELightType;
	class CommandPool;
	class Buffer;
	struct Light;

	class LightsManager
	{
	private:
		std::vector<Light*> m_lights;
		int m_nbOfLights = 0;

		std::vector<Buffer> m_lightBuffers;
		std::vector<Buffer> m_nbLightBuffers;
	
	public:
		LightsManager() = delete;
		LightsManager(const LightsManager& other) = delete;
		LightsManager(LightsManager&& other) noexcept = delete;
		LightsManager& operator=(const LightsManager& other) = delete;
		LightsManager& operator=(LightsManager&& other) noexcept = delete;
		~LightsManager() = default;
		
		LightsManager(const CommandPool& commandPool); 

		RENDERING_LIBRARY_API Light* AddLight(ELightType lightType);

		RENDERING_LIBRARY_API void DestroyLight(Light* light);

		[[nodiscard]] const std::vector<Buffer>& GetLightsBuffers() const; 
		[[nodiscard]] const std::vector<Buffer>& GetNbOfLightsBuffers() const;
		
		void UpdateLights(uint32_t imageIndex);

	private:
		void CreateLightsBuffer(const CommandPool& commandPool);
	};
}
