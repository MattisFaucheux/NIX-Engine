#include "pch.h"

#include "RenderPass/OffScreenRenderPass.h"

#include <array>

#include "Memory/Buffer.h"
#include "Camera.h"
#include "Context/Device/Device.h"
#include "Tools/Initializers.h"
#include "SwapChain/SwapChain.h"
#include "GraphicsPipeline.h"
#include "Log/Logger.h"
#include "UI/UIManager.h"
#include "Material/TextureSampler.h"

NIX::Rendering::OffScreenRenderPass::~OffScreenRenderPass()
{
    Cleanup();
}

void NIX::Rendering::OffScreenRenderPass::Initialize(const SwapChain& swapChain)
{
    m_width = OFF_SCREEN_DEFAULT_WIDTH;
    m_height = OFF_SCREEN_DEFAULT_HEIGHT;

    VkFormat fbDepthFormat = Device::GetSingleton()->FindDepthFormat();

	
    m_colors.resize(swapChain.GetSwapChainImages().size());

    VkImageCreateInfo image = Initializers::ImageCreateInfo();
    image.imageType = VK_IMAGE_TYPE_2D;
    image.format = OFF_SCREEN_IMAGE_FORMAT;
    image.extent.width = m_width;
    image.extent.height = m_height;
    image.extent.depth = 1;
    image.mipLevels = 1;
    image.arrayLayers = 1;
    image.samples = VK_SAMPLE_COUNT_1_BIT;
    image.tiling = VK_IMAGE_TILING_OPTIMAL;
    // We will sample directly from the color attachment
    image.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

    VkMemoryAllocateInfo memAlloc = Initializers::MemoryAllocateInfo();
    VkMemoryRequirements memReqs;

    for (int i = 0; i < swapChain.GetSwapChainImages().size(); i++)
    {
        if (vkCreateImage(Device::GetSingleton()->GetVkLogicalDevice(), &image, nullptr, &m_colors[i].image) != VK_SUCCESS)
        {
            ASSERT(SOURCE_INFO, "Failed to create Image for offscreen rendering !");
        }

        vkGetImageMemoryRequirements(Device::GetSingleton()->GetVkLogicalDevice(), m_colors[i].image, &memReqs);
        memAlloc.allocationSize = memReqs.size;
        memAlloc.memoryTypeIndex = Buffer::FindMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        if (vkAllocateMemory(Device::GetSingleton()->GetVkLogicalDevice(), &memAlloc, nullptr, &m_colors[i].mem) != VK_SUCCESS)
        {
            ASSERT(SOURCE_INFO, "Failed to allocate memory for offscreen rendering !");
        }

        if (vkBindImageMemory(Device::GetSingleton()->GetVkLogicalDevice(), m_colors[i].image, m_colors[i].mem, 0) != VK_SUCCESS)
        {
            ASSERT(SOURCE_INFO, "Failed to bind image memory for offscreen rendering !");
        }

        VkImageViewCreateInfo colorImageView = Initializers::ImageViewCreateInfo();
        colorImageView.viewType = VK_IMAGE_VIEW_TYPE_2D;
        colorImageView.format = OFF_SCREEN_IMAGE_FORMAT;
        colorImageView.subresourceRange = {};
        colorImageView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        colorImageView.subresourceRange.baseMipLevel = 0;
        colorImageView.subresourceRange.levelCount = 1;
        colorImageView.subresourceRange.baseArrayLayer = 0;
        colorImageView.subresourceRange.layerCount = 1;
        colorImageView.image = m_colors[i].image;

        if (vkCreateImageView(Device::GetSingleton()->GetVkLogicalDevice(), &colorImageView, nullptr, &m_colors[i].view) != VK_SUCCESS)
        {
            ASSERT(SOURCE_INFO, "Failed to create Image view for offscreen rendering !");
        }

    }

    // Depth stencil attachment
    image.format = fbDepthFormat;
    image.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

    if (vkCreateImage(Device::GetSingleton()->GetVkLogicalDevice(), &image, nullptr, &m_depth.image) != VK_SUCCESS)
    {
        ASSERT(SOURCE_INFO, "Failed to create image depth stencil for offscreen rendering !");
    }

    vkGetImageMemoryRequirements(Device::GetSingleton()->GetVkLogicalDevice(), m_depth.image, &memReqs);
    memAlloc.allocationSize = memReqs.size;
    memAlloc.memoryTypeIndex = Buffer::FindMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    if (vkAllocateMemory(Device::GetSingleton()->GetVkLogicalDevice(), &memAlloc, nullptr, &m_depth.mem) != VK_SUCCESS)
    {
        ASSERT(SOURCE_INFO, "Failed to allocate memory for offscreen rendering !");
    }

    if (vkBindImageMemory(Device::GetSingleton()->GetVkLogicalDevice(), m_depth.image, m_depth.mem, 0) != VK_SUCCESS)
    {
        ASSERT(SOURCE_INFO, "Failed to bind image memory for offscreen rendering !");
    }

    VkImageViewCreateInfo depthStencilView = Initializers::ImageViewCreateInfo();
    depthStencilView.viewType = VK_IMAGE_VIEW_TYPE_2D;
    depthStencilView.format = fbDepthFormat;
    depthStencilView.flags = 0;
    depthStencilView.subresourceRange = {};
    depthStencilView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    depthStencilView.subresourceRange.baseMipLevel = 0;
    depthStencilView.subresourceRange.levelCount = 1;
    depthStencilView.subresourceRange.baseArrayLayer = 0;
    depthStencilView.subresourceRange.layerCount = 1;
    depthStencilView.image = m_depth.image;

    if (vkCreateImageView(Device::GetSingleton()->GetVkLogicalDevice(), &depthStencilView, nullptr, &m_depth.view) != VK_SUCCESS)
    {
        ASSERT(SOURCE_INFO, "Failed to create image view for offscreen rendering !");
    }

    // Create a separate render pass for the offscreen rendering as it may differ from the one used for scene rendering

    std::array<VkAttachmentDescription, 2> attchmentDescriptions = {};
    // Color attachment
    attchmentDescriptions[0].format = OFF_SCREEN_IMAGE_FORMAT;
    attchmentDescriptions[0].samples = VK_SAMPLE_COUNT_1_BIT;
    attchmentDescriptions[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attchmentDescriptions[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attchmentDescriptions[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attchmentDescriptions[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attchmentDescriptions[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attchmentDescriptions[0].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    // Depth attachment
    attchmentDescriptions[1].format = fbDepthFormat;
    attchmentDescriptions[1].samples = VK_SAMPLE_COUNT_1_BIT;
    attchmentDescriptions[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attchmentDescriptions[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attchmentDescriptions[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attchmentDescriptions[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attchmentDescriptions[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attchmentDescriptions[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorReference = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
    VkAttachmentReference depthReference = { 1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };

    VkSubpassDescription subpassDescription = {};
    subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDescription.colorAttachmentCount = 1;
    subpassDescription.pColorAttachments = &colorReference;
    subpassDescription.pDepthStencilAttachment = &depthReference;

    // Use subpass dependencies for layout transitions
    std::array<VkSubpassDependency, 2> dependencies;

    dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[0].dstSubpass = 0;
    dependencies[0].srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[0].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
    dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    dependencies[1].srcSubpass = 0;
    dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    // Create the actual renderpass
    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attchmentDescriptions.size());
    renderPassInfo.pAttachments = attchmentDescriptions.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpassDescription;
    renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
    renderPassInfo.pDependencies = dependencies.data();

    if (vkCreateRenderPass(Device::GetSingleton()->GetVkLogicalDevice(), &renderPassInfo, nullptr, &m_renderPass) != VK_SUCCESS)
    {
        ASSERT(SOURCE_INFO, "Failed to create offscreen render pass !");
    }

    m_frameBuffers.resize(swapChain.GetSwapChainImages().size());

    for (int i = 0; i < swapChain.GetSwapChainImages().size(); i++)
    {

        VkImageView attachments[2];
        attachments[0] = m_colors[i].view;
        attachments[1] = m_depth.view;

        VkFramebufferCreateInfo fbufCreateInfo = Initializers::FrameBufferCreateInfo();
        fbufCreateInfo.renderPass = m_renderPass;
        fbufCreateInfo.attachmentCount = 2;
        fbufCreateInfo.pAttachments = attachments;
        fbufCreateInfo.width = m_width;
        fbufCreateInfo.height = m_height;
        fbufCreateInfo.layers = 1;

        if (vkCreateFramebuffer(Device::GetSingleton()->GetVkLogicalDevice(), &fbufCreateInfo, nullptr, &m_frameBuffers[i]) != VK_SUCCESS)
        {
            ASSERT(SOURCE_INFO, "Failed to create offscreen frame buffer !");
        }

        m_isTextureIdSet.assign(swapChain.GetSwapChainImages().size(), false);
    }
}

void NIX::Rendering::OffScreenRenderPass::Cleanup()
{
    for (auto* frameBuffer : m_frameBuffers)
    {
        vkDestroyFramebuffer(Device::GetSingleton()->GetVkLogicalDevice(), frameBuffer, nullptr);
    }
	
    vkDestroyRenderPass(Device::GetSingleton()->GetVkLogicalDevice(), m_renderPass, nullptr);

}

void NIX::Rendering::OffScreenRenderPass::Update(uint32_t imageIndex, VkCommandBuffer commandBuffer,
                                                    UIManager& uiManager, std::vector<DrawableObject*>& drawableObjects,
                                                    Camera& camera, const std::vector<Buffer>& lightBuffers, const std::vector<Buffer>& nbOfLightsBuffers,
                                                    const TextureSampler& textureSampler/*, int width, int height*/)
{    
    BeginRenderPass(imageIndex, commandBuffer);
	
    GraphicsPipeline* currentPipeline = nullptr;
	
    for (DrawableObject* drawableObject : drawableObjects)
    {
        if (drawableObject == nullptr || !drawableObject->GetIsActive())
        {
            continue;
        }

        drawableObject->UpdateTransformationBuffer(imageIndex);
    	
        if (drawableObject->GetPipeline() != currentPipeline)
        {
            currentPipeline = drawableObject->GetPipeline();

        	if (currentPipeline == nullptr)
        	{
                continue;
        	}
            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, drawableObject->GetPipeline()->GetVkPipeline());

            BindDescriptorSetsByFrame(imageIndex, commandBuffer, camera, lightBuffers, nbOfLightsBuffers, *currentPipeline);
            
        }

    	if (currentPipeline != nullptr)
    	{
            BindDescriptorSetsByMaterial(imageIndex, commandBuffer, textureSampler, drawableObject->GetMaterial(), *currentPipeline);
    	}
           	
        if (drawableObject->IsDescriptorSet())
        {
            BindDescriptorSetsByModel(imageIndex, commandBuffer, drawableObject->GetTransformationBuffer(), currentPipeline->GetVkPipelineLayout(), drawableObject->GetDescriptorSets());
        }
    	
        
        drawableObject->BindVertexBuffer(commandBuffer);

    	if (drawableObject->IsIndex())
    	{
            drawableObject->BindIndexBuffer(commandBuffer);
            drawableObject->DrawIndex(commandBuffer);
    	}
        else
        {
            drawableObject->DrawVertex(commandBuffer);
        }
    }

    vkCmdEndRenderPass(commandBuffer);

    if (!m_isTextureIdSet[imageIndex])
    {
        uiManager.SetTextureIdScene(imageIndex, m_colors[imageIndex].view);
        m_isTextureIdSet[imageIndex] = true;
    }
}


NIX::Rendering::FrameBufferAttachment::~FrameBufferAttachment()
{
    vkFreeMemory(Device::GetSingleton()->GetVkLogicalDevice(), mem, nullptr);
    vkDestroyImage(Device::GetSingleton()->GetVkLogicalDevice(), image, nullptr);
    vkDestroyImageView(Device::GetSingleton()->GetVkLogicalDevice(), view, nullptr);
}

void NIX::Rendering::OffScreenRenderPass::BeginRenderPass(uint32_t imageIndex, VkCommandBuffer commandBuffer)
{	
    std::array<VkClearValue, 2> clearValues{};

    clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
    clearValues[1].depthStencil = { 1.0f, 0 };

    VkRenderPassBeginInfo renderPassInfo = Initializers::RenderPassBeginInfo();
    renderPassInfo.renderPass = m_renderPass;
    renderPassInfo.framebuffer = m_frameBuffers[imageIndex];
    renderPassInfo.renderArea.extent.width = m_width;
    renderPassInfo.renderArea.extent.height = m_height;
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    SetViewport(commandBuffer);
    SetScissor(commandBuffer);
}

void NIX::Rendering::OffScreenRenderPass::SetViewport(VkCommandBuffer commandBuffer)
{
    VkViewport viewport = Initializers::Viewport((float)m_width, (float)m_height, 0.0f, 1.0f);
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
}

void NIX::Rendering::OffScreenRenderPass::SetScissor(VkCommandBuffer commandBuffer)
{
    VkRect2D scissor = Initializers::Rect2D(m_width, m_height, 0, 0);
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void NIX::Rendering::OffScreenRenderPass::BindDescriptorSetsByFrame(uint32_t imageIndex, VkCommandBuffer commandBuffer, Camera& camera, const std::vector<Buffer>& lightBuffers, const std::vector<Buffer>& nbOfLights, GraphicsPipeline& graphicsPipeline)
{
    if (graphicsPipeline.GetNbOfLayout() <= 0)
    {
	    return;
    }
	
    camera.UpdateCameraBuffer(imageIndex, m_sceneWindowWidth, m_sceneWindowHeight);
    
    std::vector<VkWriteDescriptorSet> descriptorWrites;

    VkDescriptorBufferInfo cameraInfo;

    if (!graphicsPipeline.GetDescriptorSets().IsUpdate(imageIndex))
    {
        if (graphicsPipeline.GetNbOfLayout() > 0)
        {
            camera.GetBuffer(imageIndex).InitDescriptorBufferInfo(cameraInfo);

            descriptorWrites.emplace_back();

            descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[0].dstSet = graphicsPipeline.GetDescriptorSets().GetVkDescriptorSet(imageIndex);
            descriptorWrites[0].dstBinding = 0;
            descriptorWrites[0].dstArrayElement = 0;
            descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrites[0].descriptorCount = 1;
            descriptorWrites[0].pBufferInfo = &cameraInfo;
        }

        VkDescriptorBufferInfo lightInfo;
        if (graphicsPipeline.GetNbOfLayout() > 1)
        {
            lightBuffers[imageIndex].InitDescriptorBufferInfo(lightInfo);

            descriptorWrites.emplace_back();

            descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[1].dstSet = graphicsPipeline.GetDescriptorSets().GetVkDescriptorSet(imageIndex);
            descriptorWrites[1].dstBinding = 1;
            descriptorWrites[1].dstArrayElement = 0;
            descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrites[1].descriptorCount = 1;
            descriptorWrites[1].pBufferInfo = &lightInfo;
        }

        VkDescriptorBufferInfo nbLightInfo;
        if (graphicsPipeline.GetNbOfLayout() > 2)
        {
            nbOfLights[imageIndex].InitDescriptorBufferInfo(nbLightInfo);

            descriptorWrites.emplace_back();
            descriptorWrites[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[2].dstSet = graphicsPipeline.GetDescriptorSets().GetVkDescriptorSet(imageIndex);
            descriptorWrites[2].dstBinding = 2;
            descriptorWrites[2].dstArrayElement = 0;
            descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrites[2].descriptorCount = 1;
            descriptorWrites[2].pBufferInfo = &nbLightInfo;

        }
    	
        vkUpdateDescriptorSets(Device::GetSingleton()->GetVkLogicalDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

        graphicsPipeline.GetDescriptorSets().SetIsUpdate(imageIndex, true);
    }
	
    
    
   
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.GetVkPipelineLayout(), 0, 1, &graphicsPipeline.GetDescriptorSets().GetVkDescriptorSet(imageIndex), 0, nullptr);

}

void NIX::Rendering::OffScreenRenderPass::BindDescriptorSetsByMaterial(uint32_t imageIndex, VkCommandBuffer commandBuffer, const TextureSampler& textureSampler, Material* material, const GraphicsPipeline& graphicsPipeline)
{
    if (material == nullptr || material->m_descriptorSets.IsEmpty())
    {
        return;
    }

	if (!material->m_descriptorSets.IsUpdate(imageIndex))
	{
        VkDescriptorImageInfo imageInfo;
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = material->albedoMap->GetVkImageView();
        imageInfo.sampler = textureSampler.GetVkTextureSampler();

        std::array<VkWriteDescriptorSet, 1> descriptorWrites{};

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = material->m_descriptorSets.GetVkDescriptorSet(imageIndex);
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(Device::GetSingleton()->GetVkLogicalDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

        material->m_descriptorSets.SetIsUpdate(imageIndex, true);
	}
    
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.GetVkPipelineLayout(), 1, 1, &material->m_descriptorSets.GetVkDescriptorSet(imageIndex), 0, nullptr);

    
}

void NIX::Rendering::OffScreenRenderPass::BindDescriptorSetsByModel(uint32_t imageIndex, VkCommandBuffer commandBuffer, const std::vector<Buffer>& modelMatBuffers, VkPipelineLayout pipelineLayout, DescriptorSets& descriptorSets)
{

    if (!descriptorSets.IsUpdate(imageIndex))
    {
        VkDescriptorBufferInfo modelMatInfo;
        modelMatBuffers[imageIndex].InitDescriptorBufferInfo(modelMatInfo);

        std::array<VkWriteDescriptorSet, 1> descriptorWrites{};

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = descriptorSets.GetVkDescriptorSet(imageIndex);
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &modelMatInfo;

        vkUpdateDescriptorSets(Device::GetSingleton()->GetVkLogicalDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

        descriptorSets.SetIsUpdate(imageIndex, true);
    }
	
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 2, 1, &descriptorSets.GetVkDescriptorSet(imageIndex), 0, nullptr);
}

VkRenderPass NIX::Rendering::OffScreenRenderPass::GetVkRenderPass() const
{
    return m_renderPass;
}

void NIX::Rendering::OffScreenRenderPass::UpdateSceneWindowSize(const float width, float height)
{
    m_sceneWindowWidth = width;
    m_sceneWindowHeight = height;
}
