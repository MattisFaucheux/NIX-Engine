#include "pch.h"

#include "RenderPass/RenderPass.h"
#include "GraphicsPipeline.h"
#include "Tools/Initializers.h"
#include "SwapChain/SwapChain.h"
#include "Context/Device/Device.h"
#include "UI/UIManager.h"

#include <array>
#include <stdexcept>

#include "Log/Logger.h"

namespace NIX::Rendering
{
    RenderPass::~RenderPass()
    {
        Cleanup();
    }
	
    void RenderPass::Initialize(const SwapChain& swapChain)
	{
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = swapChain.GetImageFormat();
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentDescription depthAttachment{};
        depthAttachment.format = Device::GetSingleton()->FindDepthFormat();
        depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference depthAttachmentRef{};
        depthAttachmentRef.attachment = 1;
        depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;
        subpass.pDepthStencilAttachment = &depthAttachmentRef;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcAccessMask = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
        VkRenderPassCreateInfo renderPassInfo = Initializers::RenderPassCreateInfo();
        renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        renderPassInfo.pAttachments = attachments.data();
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        if (vkCreateRenderPass(Device::GetSingleton()->GetVkLogicalDevice(), &renderPassInfo, nullptr, &m_renderPass) != VK_SUCCESS)
        {
            ASSERT(SOURCE_INFO, "Failed to create render pass !");
        }
	}

    void RenderPass::Update(uint32_t imageIndex, VkCommandBuffer commandBuffer, const SwapChain& swapChain,
	    const GraphicsPipeline& graphicsPipeline, UIManager& uiManager)
    {
        VkRenderPassBeginInfo renderPassInfo = Initializers::RenderPassBeginInfo();
        renderPassInfo.renderPass = m_renderPass;
        renderPassInfo.framebuffer = swapChain.GetFrameBuffers()[imageIndex];
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = swapChain.GetExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        clearValues[1].depthStencil = { 1.0f, 0 };

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);


        VkViewport viewport = Initializers::Viewport(static_cast<float>(swapChain.GetExtent().width), static_cast<float>(swapChain.GetExtent().height), 0.0f, 1.0f);
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRect2D scissor = Initializers::Rect2D(swapChain.GetExtent().width, swapChain.GetExtent().height, 0, 0);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.GetVkPipeline());

        uiManager.RenderDrawData(commandBuffer);

        vkCmdEndRenderPass(commandBuffer);
    }

    void RenderPass::Cleanup()
    {
        vkDestroyRenderPass(Device::GetSingleton()->GetVkLogicalDevice(), m_renderPass, nullptr);
    }

    VkRenderPass RenderPass::GetVkRenderPass() const
    {
        return m_renderPass;
    }
}
