#include "pch.h"

#include "Descriptors/DescriptorSets.h"
#include "Descriptors/DescriptorSetLayout.h"
#include "Descriptors/DescriptorPool.h"

#include "Tools/Initializers.h"
#include "Context/Device/Device.h"
#include "SwapChain/SwapChain.h"

#include <stdexcept>

#include "Log/Logger.h"

namespace NIX::Rendering
{
	/*DescriptorSets::~DescriptorSets()
	{
        for (auto* descriptorSet : m_descriptorSets)
        {
            if (m_descriptorPool != nullptr)
            {
                vkFreeDescriptorSets(Device::GetSingleton()->GetVkLogicalDevice(), m_descriptorPool->GetVkDescriptorPool(), 1, &descriptorSet);
            }
        }
	}*/

    void DescriptorSets::Initialize(const DescriptorSetLayout& descriptorSetLayout,
	                                DescriptorPool& descriptorPool)
    {
        std::vector<VkDescriptorSetLayout> layouts(MIN_IMAGE_COUNT, descriptorSetLayout.GetVkDescriptorSetLayout());
        VkDescriptorSetAllocateInfo allocInfo = Initializers::DescriptorSetAllocateInfo(descriptorPool.GetVkDescriptorPool(), layouts.data(), MIN_IMAGE_COUNT);

        m_descriptorSets.resize(MIN_IMAGE_COUNT);
        if (vkAllocateDescriptorSets(Device::GetSingleton()->GetVkLogicalDevice(), &allocInfo, m_descriptorSets.data()) != VK_SUCCESS)
        {
            ASSERT(SOURCE_INFO, "Failed to allocate descriptor sets !");
        }

        m_isUpdate.resize(MIN_IMAGE_COUNT, false);

        //m_descriptorPool = &descriptorPool;
    }
    bool DescriptorSets::IsUpdate(uint32_t imageIndex) const
    {
        return m_isUpdate[imageIndex];
    }

    void DescriptorSets::SetIsUpdate(uint32_t imageIndex, bool value)
    {
        m_isUpdate[imageIndex] = value;
    }

    void DescriptorSets::SetAllIsUpdate(bool value)
    {
    	for (auto&& isUpdate : m_isUpdate)
        {
            isUpdate = value;
    	}
    }

    bool DescriptorSets::IsEmpty() const
    {
        return m_descriptorSets.empty();
    }

    VkDescriptorSet& DescriptorSets::GetVkDescriptorSet(uint32_t imageIndex)
    {
		return m_descriptorSets[imageIndex];
    }
}
