#include "pch.h"

#include "Tools/Initializers.h"
#include "Tools/Version.h"
#include "CommandPool.h"
#include "Descriptors/DescriptorSets.h"

namespace NIX::Rendering
{
    VkApplicationInfo Initializers::ApplicationInfo(const std::string& appName,
        const Version& appVersion, const std::string& engineName, const Version& engineVersion)
    {
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = appName.c_str();
        appInfo.applicationVersion = VK_MAKE_VERSION(appVersion.first, appVersion.second, appVersion.third);
        appInfo.pEngineName = engineName.c_str();
        appInfo.engineVersion = VK_MAKE_VERSION(engineVersion.first, engineVersion.second, engineVersion.third);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        return appInfo;
    }

    VkInstanceCreateInfo Initializers::InstanceCreateInfo(const VkApplicationInfo& appInfo)
    {
        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
    	
        return createInfo;
    }

    VkSubmitInfo Initializers::SubmitInfo()
    {
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        return submitInfo;
    }

    VkPresentInfoKHR Initializers::PresentInfo()
    {
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    	
        return presentInfo;
    }

    VkCommandPoolCreateInfo Initializers::CommandPoolCreateInfo()
    {
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;

        return poolInfo;
    }

    VkCommandBufferAllocateInfo Initializers::CommandBufferAllocateInfo(const CommandPool& commandPool,
                                                                        VkCommandBufferLevel level, uint32_t bufferCount)
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = commandPool.GetCommandPool();
        allocInfo.level = level;
        allocInfo.commandBufferCount = bufferCount;

        return allocInfo;
    }

    VkCommandBufferBeginInfo Initializers::CommandBufferBeginInfo()
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        return beginInfo;
    }

    VkRenderPassBeginInfo Initializers::RenderPassBeginInfo()
    {
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;

        return renderPassInfo;
    }

    VkRenderPassCreateInfo Initializers::RenderPassCreateInfo()
    {
        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

        return renderPassInfo;
    }

    VkImageCreateInfo Initializers::ImageCreateInfo()
    {
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;

        return imageInfo;
    }

    VkImageViewCreateInfo Initializers::ImageViewCreateInfo()
    {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;

        return viewInfo;
    }

    VkImageMemoryBarrier Initializers::ImageMemoryBarrier()
    {
        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;

        return barrier;
    }

    VkDescriptorSetAllocateInfo Initializers::DescriptorSetAllocateInfo(VkDescriptorPool descriptorPool,
                                                                        const VkDescriptorSetLayout* pSetLayouts, uint32_t descriptorSetCount)
    {
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.pSetLayouts = pSetLayouts;
        allocInfo.descriptorSetCount = descriptorSetCount;

        return allocInfo;
    }

    VkDescriptorPoolCreateInfo Initializers::DescriptorPoolCreateInfo(const std::vector<VkDescriptorPoolSize>& poolSizes, uint32_t maxSets)
    {
        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        poolInfo.poolSizeCount = static_cast<uint32_t>(std::size(poolSizes));
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = maxSets;

        return poolInfo;
    }

    VkDescriptorSetLayoutCreateInfo Initializers::DescriptorSetLayoutCreateInfo()
    {
        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;

        return layoutInfo;
    }

    VkMemoryAllocateInfo Initializers::MemoryAllocateInfo()
    {
        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;

        return allocInfo;
    }

    VkSamplerCreateInfo Initializers::SamplerCreateInfo()
    {
        VkSamplerCreateInfo samplerCreateInfo{};
        samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerCreateInfo.maxAnisotropy = 1.0f;
    	
        return samplerCreateInfo;
    }

    VkFramebufferCreateInfo Initializers::FrameBufferCreateInfo()
    {
        VkFramebufferCreateInfo framebufferCreateInfo{};
        framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    	
        return framebufferCreateInfo;
    }

    VkBufferCreateInfo Initializers::BufferCreateInfo(VkDeviceSize size, VkBufferUsageFlags usage,
	    VkSharingMode sharingMode)
    {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = sharingMode;
    
        return bufferInfo;
    }

    VkViewport Initializers::Viewport(float width, float height, float minDepth, float maxDepth)
    {
        VkViewport viewport{};
        viewport.width = width;
        viewport.height = height;
        viewport.minDepth = minDepth;
        viewport.maxDepth = maxDepth;
    	
        return viewport;
    }

    VkRect2D Initializers::Rect2D(int32_t width, int32_t height, int32_t offsetX, int32_t offsetY)
    {
        VkRect2D rect2D{};
        rect2D.extent.width = width;
        rect2D.extent.height = height;
        rect2D.offset.x = offsetX;
        rect2D.offset.y = offsetY;
    	
        return rect2D;
    }

    VkShaderModuleCreateInfo Initializers::ShaderModuleCreateInfo(size_t codeSize)
    {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = codeSize;

        return createInfo;
    }

    VkDeviceQueueCreateInfo Initializers::DeviceQueueCreateInfo()
    {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;

        return queueCreateInfo;
    }

    VkDeviceCreateInfo Initializers::DeviceCreateInfo()
    {
        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        return createInfo;
    }

    VkSwapchainCreateInfoKHR Initializers::SwapChainCreateInfoKHR(VkSurfaceKHR surface)
    {
        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = surface;

        return createInfo;
    }

    VkPipelineShaderStageCreateInfo Initializers::PipelineShaderStageCreateInfo()
    {
        VkPipelineShaderStageCreateInfo shaderStageInfo{};
        shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

        return shaderStageInfo;
    }

    VkPipelineVertexInputStateCreateInfo Initializers::PipelineVertexInputStateCreateInfo()
    {
        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

        return vertexInputInfo;
    }

    VkPipelineInputAssemblyStateCreateInfo Initializers::PipelineInputAssemblyStateCreateInfo()
    {
        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;

        return inputAssembly;
    }

    VkPipelineViewportStateCreateInfo Initializers::PipelineViewportStateCreateInfo()
    {
        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;

        return viewportState;
    }

    VkPipelineRasterizationStateCreateInfo Initializers::PipelineRasterizationStateCreateInfo()
    {
        VkPipelineRasterizationStateCreateInfo rasterizerInfo{};
        rasterizerInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;

        return rasterizerInfo;
    }

    VkPipelineMultisampleStateCreateInfo Initializers::PipelineMultisampleStateCreateInfo()
    {
        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;

        return multisampling;
    }

    VkPipelineDepthStencilStateCreateInfo Initializers::PipelineDepthStencilStateCreateInfo()
    {
        VkPipelineDepthStencilStateCreateInfo depthStencil{};
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;

        return  depthStencil;
    }

    VkPipelineColorBlendStateCreateInfo Initializers::PipelineColorBlendStateCreateInfo()
    {
        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;

        return colorBlending;
    }

    VkPipelineLayoutCreateInfo Initializers::PipelineLayoutCreateInfo()
    {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

        return pipelineLayoutInfo;
    }

    VkPipelineDynamicStateCreateInfo Initializers::PipelineDynamicStateCreateInfo()
    {
        VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo{};
        dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;

        return  dynamicStateCreateInfo;
    }

    VkGraphicsPipelineCreateInfo Initializers::GraphicsPipelineCreateInfo()
    {
        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

        return pipelineInfo;
    }
}
