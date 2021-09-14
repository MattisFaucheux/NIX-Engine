#include "pch.h"

#include "Light/LightsManager.h"



#include "SwapChain/SwapChain.h"
#include "CommandPool.h"
#include "Memory/Buffer.h"
#include "Context/Device/Device.h"

#include "Memory/EBufferType.h"
#include "Light/ELightType.h"
#include "Light/Light.h"

#include "Vector/Vector2.h"

#include <vulkan/vulkan_core.h>

#include "DrawableObject/DrawableObject.h"
#include "Log/Logger.h"

namespace NIX::Rendering
{

	LightsManager::LightsManager(const CommandPool& commandPool)
	{
		CreateLightsBuffer(commandPool);

		m_lights.reserve(NB_LIGHTS_MAX);
	}

	Light* LightsManager::AddLight(ELightType lightType)
	{
		Light* light = new Light(lightType);

		if (light == nullptr)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "Failed to create a light");
			return nullptr;
		}
		
		m_lights.emplace_back(light);

		return light;
	}

	void LightsManager::DestroyLight(Light* light)
	{		
		m_lights.erase(std::remove_if(m_lights.begin(), m_lights.end(), [&light](Light* i) { return (i == light); }));
		delete light;
	}

	const std::vector<Buffer>& LightsManager::GetLightsBuffers() const
	{
		return m_lightBuffers;
	}

	const std::vector<Buffer>& LightsManager::GetNbOfLightsBuffers() const
	{
		return m_nbLightBuffers;
	}

	void LightsManager::CreateLightsBuffer(const CommandPool& commandPool)
	{
		m_lightBuffers.resize(MIN_IMAGE_COUNT);

		VkDeviceSize  lightBufferSize = sizeof(Light) * NB_LIGHTS_MAX;

		for (size_t i = 0; i < MIN_IMAGE_COUNT; i++)
		{
			m_lightBuffers[i].Initialize(commandPool, lightBufferSize, nullptr, EBufferType::UNIFORM);
		}

		m_nbLightBuffers.resize(MIN_IMAGE_COUNT);

		VkDeviceSize nbLightSize = sizeof(int);

		for (size_t nbImage = 0; nbImage < MIN_IMAGE_COUNT; nbImage++)
		{
			m_nbLightBuffers[nbImage].Initialize(commandPool, nbLightSize, m_nbLightBuffers.data(), EBufferType::UNIFORM);
		}
	}

	void LightsManager::UpdateLights(uint32_t imageIndex)
	{
		
		int lightCount = 0;
		std::vector<Light> lights;
		lights.reserve(NB_LIGHTS_MAX);
		
		for (Light* light : m_lights)
		{
			if (light->isActive)
			{
				lights.emplace_back(Light(*light));
				lightCount++;
			}
		}

		m_nbOfLights = lightCount;

		const VkDeviceSize lightInfoSize = sizeof(Light) * m_nbOfLights;
		
		if (lightInfoSize != 0)
		{
			m_lightBuffers[imageIndex].Update(lightInfoSize, lights.data());
		}
		
		m_nbLightBuffers[imageIndex].Update(sizeof(int), &m_nbOfLights);
	}
}
