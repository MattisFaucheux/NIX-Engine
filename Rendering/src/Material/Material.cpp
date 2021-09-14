#include "pch.h"

#include "Material/Material.h"
#include "Material/TextureManager.h"

#include <stdexcept>


#include "Context/Context.h"
#include "Descriptors/DescriptorSetLayout.h"
#include "Descriptors/DescriptorSets.h"
#include "SwapChain/SwapChain.h"

#include "Shaders/ShaderStage.h"

namespace NIX::Rendering
{
	Material::Material()
	{
		InitializeDescriptorSets();
	}

	Material::Material(TextureView* albedo, TextureView* normal)
		: albedoMap(albedo), normalMap(normal)
	{
		if (normalMap != nullptr)
		{
			isNormalMap = true;
		}

        InitializeDescriptorSets();
	}

	void Material::InitializeDescriptorSets()
	{

		
		std::vector<VkDescriptorPoolSize> materialPoolSizes = 
		{
			{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, MIN_IMAGE_COUNT }
		};
        

        m_descriptorSetLayout.AddLayoutBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, ShaderStage::FRAGMENT);
        m_descriptorSetLayout.Initialize();

		m_descriptorPool.Initialize(materialPoolSizes, MIN_IMAGE_COUNT);
        
        m_descriptorSets.Initialize(m_descriptorSetLayout, m_descriptorPool);

	}

	TextureView* Material::CreateMaterialPreview()
	{
		//TODO send a preview of the mat on a sphere
		// or all texture composing the mat instead of just the albedo
		return new TextureView(*albedoMap);
	}

	void Material::UpdateAlbedoMap(TextureView* albedo)
	{
		albedoMap = albedo;
		m_descriptorSets.SetAllIsUpdate(false);
	}

	void Material::UpdateNormalMap(TextureView* normal)
	{
		normalMap = normal;

		if (normalMap == nullptr)
		{
			isNormalMap = false;
		}
		
		m_descriptorSets.SetAllIsUpdate(false);
	}
}


