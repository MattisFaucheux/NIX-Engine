#include "pch.h"

#include "CommandBuffers.h"
#include "Tools/Initializers.h"
#include "Context/Device/Device.h"
#include "CommandPool.h"
#include "SwapChain/SwapChain.h"

#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_vulkan.h>

#include <stdexcept>

#include "Log/Logger.h"

void NIX::Rendering::CommandBuffers::Initialize(CommandPool& commandPool)
{
    m_commandBuffers.resize(MIN_IMAGE_COUNT);

    VkCommandBufferAllocateInfo allocInfo = Initializers::CommandBufferAllocateInfo(commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, static_cast<uint32_t>(m_commandBuffers.size()));
    if (vkAllocateCommandBuffers(Device::GetSingleton()->GetVkLogicalDevice(), &allocInfo, m_commandBuffers.data()) != VK_SUCCESS)
    {
        ASSERT(SOURCE_INFO, "Failed to allocate command buffers !");
    }

    m_commandPool = &commandPool;
}

void NIX::Rendering::CommandBuffers::Cleanup()
{
    vkFreeCommandBuffers(Device::GetSingleton()->GetVkLogicalDevice(), m_commandPool->GetCommandPool(), static_cast<uint32_t>(m_commandBuffers.size()), m_commandBuffers.data());
}

NIX::Rendering::CommandBuffers::~CommandBuffers()
{
    if (m_commandPool != nullptr)
    {
        vkFreeCommandBuffers(Device::GetSingleton()->GetVkLogicalDevice(), m_commandPool->GetCommandPool(), static_cast<uint32_t>(m_commandBuffers.size()), m_commandBuffers.data());
    }
}

void NIX::Rendering::CommandBuffers::StartUpdate(uint32_t imageIndex)
{
    VkCommandBufferBeginInfo beginInfo = Initializers::CommandBufferBeginInfo();
	
    if (vkBeginCommandBuffer(m_commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
    {
        ASSERT(SOURCE_INFO, "Failed to begin recording command buffer !");
    }
}

void NIX::Rendering::CommandBuffers::EndUpdate(uint32_t imageIndex)
{
    if (vkEndCommandBuffer(m_commandBuffers[imageIndex]) != VK_SUCCESS)
    {
        ASSERT(SOURCE_INFO, "Failed to record command buffer !");
    }
}

VkCommandBuffer NIX::Rendering::CommandBuffers::GetVkCommandBuffer(uint32_t imageIndex) const
{
    if (m_commandBuffers.size() > imageIndex)
    {
        return m_commandBuffers[imageIndex];
    }
	
    return nullptr;
}

VkCommandBuffer& NIX::Rendering::CommandBuffers::GetPtrCommandBuffer(uint32_t imageIndex)
{
    return m_commandBuffers[imageIndex];
}
