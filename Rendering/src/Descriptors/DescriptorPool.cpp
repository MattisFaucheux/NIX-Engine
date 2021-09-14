#include "pch.h"

#include "Descriptors/DescriptorPool.h"

#include "Tools/Initializers.h"
#include "Context/Device/Device.h"

#include <stdexcept>

#include "Log/Logger.h"

namespace NIX::Rendering
{
    void DescriptorPool::Initialize(const std::vector<VkDescriptorPoolSize>& poolSizes, uint32_t maxSets)
    {    	
        VkDescriptorPoolCreateInfo poolInfo = Initializers::DescriptorPoolCreateInfo(poolSizes, maxSets);

        if (vkCreateDescriptorPool(Device::GetSingleton()->GetVkLogicalDevice(), &poolInfo, nullptr, &m_descriptorPool) != VK_SUCCESS)
        {
            ASSERT(SOURCE_INFO, "Failed to create descriptor pool !");
        }
    }

    void DescriptorPool::Cleanup()
    {
        vkDestroyDescriptorPool(Device::GetSingleton()->GetVkLogicalDevice(), m_descriptorPool, nullptr);
    }

    DescriptorPool::~DescriptorPool()
    {
        Cleanup();
    }

    VkDescriptorPool DescriptorPool::GetVkDescriptorPool() const
    {
        return m_descriptorPool;
    }
}


