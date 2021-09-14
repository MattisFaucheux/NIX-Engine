#include "pch.h"

#include "Tools/Initializers.h"
#include "Context/Device/Device.h"
#include "Material/TextureSampler.h"

#include <stdexcept>

#include "Log/Logger.h"

namespace NIX::Rendering
{
    TextureSampler::~TextureSampler()
    {
        Cleanup();
    }
	
    void TextureSampler::Initialize(bool anisotropyEnable)
    {        
        VkSamplerCreateInfo samplerInfo = Initializers::SamplerCreateInfo();
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.anisotropyEnable = anisotropyEnable;
    	
        VkPhysicalDeviceProperties properties{};
        vkGetPhysicalDeviceProperties(Device::GetSingleton()->GetVkPhysicalDevice(), &properties);

        samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.mipLodBias = 0.0f;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = 0.0f;

        if (vkCreateSampler(Device::GetSingleton()->GetVkLogicalDevice(), &samplerInfo, nullptr, &m_textureSampler) != VK_SUCCESS)
        {
            ASSERT(SOURCE_INFO, "Failed to create texture sampler !");
        }
    }

    void TextureSampler::Cleanup()
    {
        vkDestroySampler(Device::GetSingleton()->GetVkLogicalDevice(), m_textureSampler, nullptr);
    }

    VkSampler TextureSampler::GetVkTextureSampler() const
    {
        return m_textureSampler;
    }
}
