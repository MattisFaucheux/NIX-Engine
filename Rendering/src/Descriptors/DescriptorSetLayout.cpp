#include "pch.h"

#include "Descriptors/DescriptorSetLayout.h"
#include "Tools/Initializers.h"
#include "Context/Device/Device.h"

#include <vulkan/vulkan_core.h>

#include <stdexcept>

#include "Log/Logger.h"

namespace NIX::Rendering
{
    void DescriptorSetLayout::Initialize()
	{    	
        VkDescriptorSetLayoutCreateInfo layoutInfo = Initializers::DescriptorSetLayoutCreateInfo();
        layoutInfo.bindingCount = static_cast<uint32_t>(m_layoutBindings.size());
        layoutInfo.pBindings = m_layoutBindings.data();

        if (vkCreateDescriptorSetLayout(Device::GetSingleton()->GetVkLogicalDevice(), &layoutInfo, nullptr, &m_descriptorSetLayout) != VK_SUCCESS)
        {
            ASSERT(SOURCE_INFO, "Failed to create descriptor set layout !");
        }
	}

    void DescriptorSetLayout::Cleanup()
    {
        vkDestroyDescriptorSetLayout(Device::GetSingleton()->GetVkLogicalDevice(), m_descriptorSetLayout, nullptr);
    }

    DescriptorSetLayout::~DescriptorSetLayout()
	{
        Cleanup();
	}

    void DescriptorSetLayout::AddLayoutBinding(int binding, VkDescriptorType type, int descriptorCount,
	    uint32_t stageFlags, const VkSampler* immutableSamplers)
    {
        VkDescriptorSetLayoutBinding layoutBinding{};
        layoutBinding.binding = binding;
        layoutBinding.descriptorType = type;
        layoutBinding.descriptorCount = descriptorCount;
        layoutBinding.stageFlags = static_cast<VkShaderStageFlags>(stageFlags);
        layoutBinding.pImmutableSamplers = immutableSamplers;
    	
        m_layoutBindings.push_back(layoutBinding);
    }

    VkDescriptorSetLayout DescriptorSetLayout::GetVkDescriptorSetLayout() const
	{
		return m_descriptorSetLayout;
	}

}
