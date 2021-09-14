#include "pch.h"

#include "GraphicsPipeline.h"
#include "Tools/Initializers.h"
#include "SwapChain/SwapChain.h"
#include "Descriptors/DescriptorSetLayout.h"
#include "Context/Device/Device.h"
#include "Shaders/Shader.h"

#include <vulkan/vulkan_core.h>

#include <fstream>
#include <stdexcept>

#include "Log/Logger.h"

namespace NIX::Rendering
{	
    void GraphicsPipeline::Initialize(const char* vertexShader, const char* fragShader, const char* geoShader, const SwapChain& swapChain, VkRenderPass renderPass)
    {    	
        InitializeShaders(vertexShader, fragShader, geoShader);

        std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
    	
        VkPipelineShaderStageCreateInfo vertShaderStageInfo = Initializers::PipelineShaderStageCreateInfo();
    	if (m_shaders.vertexShader != nullptr)
    	{
            vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
            vertShaderStageInfo.module = m_shaders.vertexShader->GetVkShader();
            vertShaderStageInfo.pName = "main";

            shaderStages.push_back(vertShaderStageInfo);
    	}

        VkPipelineShaderStageCreateInfo fragShaderStageInfo = Initializers::PipelineShaderStageCreateInfo();
    	
    	if (m_shaders.fragShader != nullptr)
    	{
            fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
            fragShaderStageInfo.module = m_shaders.fragShader->GetVkShader();
            fragShaderStageInfo.pName = "main";
            shaderStages.push_back(fragShaderStageInfo);
    	}

        VkPipelineShaderStageCreateInfo geometryShaderStageInfo = Initializers::PipelineShaderStageCreateInfo();;
        if(m_shaders.geoShader != nullptr)
        {
            geometryShaderStageInfo.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
            geometryShaderStageInfo.module = m_shaders.geoShader->GetVkShader();
            geometryShaderStageInfo.pName = "main";
            shaderStages.push_back(geometryShaderStageInfo);
        }

        VkPipelineVertexInputStateCreateInfo vertexInputInfo = Initializers::PipelineVertexInputStateCreateInfo();
        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(m_attributeDescriptions.size());
        vertexInputInfo.pVertexBindingDescriptions = &m_bindingDescription;
        vertexInputInfo.pVertexAttributeDescriptions = m_attributeDescriptions.data();

        VkPipelineInputAssemblyStateCreateInfo inputAssembly = Initializers::PipelineInputAssemblyStateCreateInfo();
        inputAssembly.topology = m_primitiveTopology;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(swapChain.GetExtent().width);
        viewport.height = static_cast<float>(swapChain.GetExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = swapChain.GetExtent();

        VkPipelineViewportStateCreateInfo viewportState = Initializers::PipelineViewportStateCreateInfo();
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

        VkPipelineRasterizationStateCreateInfo rasterizer = Initializers::PipelineRasterizationStateCreateInfo();
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;

        VkPipelineMultisampleStateCreateInfo multisampling = Initializers::PipelineMultisampleStateCreateInfo();
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampling.minSampleShading = 1.0f;
        multisampling.alphaToCoverageEnable = VK_FALSE;
        multisampling.alphaToOneEnable = VK_FALSE;

        VkPipelineDepthStencilStateCreateInfo depthStencil = Initializers::PipelineDepthStencilStateCreateInfo();
        depthStencil.depthTestEnable = VK_TRUE;
        depthStencil.depthWriteEnable = VK_TRUE;
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.stencilTestEnable = VK_FALSE;

        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

        VkPipelineColorBlendStateCreateInfo colorBlending = Initializers::PipelineColorBlendStateCreateInfo();
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;

        VkPipelineLayoutCreateInfo pipelineLayoutInfo = Initializers::PipelineLayoutCreateInfo();

        std::vector<VkDescriptorSetLayout> descriptorSets;
    	
        for (auto& descriptorSetLayout : m_descriptorSetLayouts)
        {
            descriptorSets.push_back(descriptorSetLayout.GetVkDescriptorSetLayout());
        }

        m_nbOfLayout = static_cast<int>(descriptorSets.size());
    	
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSets.size());
        pipelineLayoutInfo.pSetLayouts = descriptorSets.data();

        if (vkCreatePipelineLayout(Device::GetSingleton()->GetVkLogicalDevice(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS)
        {
            ASSERT(SOURCE_INFO, "Failed to create pipeline layout !");
        }

        std::vector<VkDynamicState> dynamicStateEnables = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
    	
        VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = Initializers::PipelineDynamicStateCreateInfo();
        dynamicStateCreateInfo.pDynamicStates = dynamicStateEnables.data();
        dynamicStateCreateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStateEnables.size());
        dynamicStateCreateInfo.flags = 0;


        VkGraphicsPipelineCreateInfo pipelineInfo = Initializers::GraphicsPipelineCreateInfo();
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages.data();
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pDepthStencilState = &depthStencil;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = &dynamicStateCreateInfo;
        pipelineInfo.layout = m_pipelineLayout;
        pipelineInfo.renderPass = renderPass;
        pipelineInfo.subpass = 0;

        if (vkCreateGraphicsPipelines(Device::GetSingleton()->GetVkLogicalDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_graphicsPipeline) != VK_SUCCESS)
        {
            ASSERT(SOURCE_INFO, "Failed to create graphics pipeline !");
        }
            	
        std::vector<VkDescriptorPoolSize> poolSizes =
        {
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
        };

        m_descriptorPool.Initialize(poolSizes, 1000);

    	if (!m_descriptorSetLayouts.empty())
    	{
            m_descriptorSets.Initialize(m_descriptorSetLayouts[0], m_descriptorPool);

    	}
    }

    void GraphicsPipeline::InitializeShaders(const char* vertexShader, const char* fragShader, const char* geoShader)
    {
    	if (vertexShader != nullptr)
    	{
            m_shaders.vertexShader = new Shader(vertexShader, EShaderType::VERTEX);

            if (m_shaders.vertexShader == nullptr)
            {
                ASSERT(SOURCE_INFO, "Failed to create vertex shader !");
            }
    	}

        if (fragShader != nullptr)
        {
            m_shaders.fragShader = new Shader(fragShader, EShaderType::FRAGMENT);

            if (m_shaders.fragShader == nullptr)
            {
                ASSERT(SOURCE_INFO, "Failed to create fragment shader !");
            }
        }

        if (geoShader != nullptr)
        {
            m_shaders.geoShader = new Shader(geoShader, EShaderType::GEOMETRY);

            if (m_shaders.geoShader == nullptr)
            {
                ASSERT(SOURCE_INFO, "Failed to create geometry shadere !");
            }
        }       
    }

    void GraphicsPipeline::InitializeDescriptorSetLayout()
    {
        for (DescriptorSetLayout& descriptorSetLayout : m_descriptorSetLayouts)
        {
			descriptorSetLayout.Initialize();
        }
    }

    void GraphicsPipeline::Cleanup()
    {
        delete m_shaders.vertexShader;
        delete m_shaders.fragShader;
        delete m_shaders.geoShader;
            	
        m_descriptorPool.Cleanup();

        vkDestroyPipeline(Device::GetSingleton()->GetVkLogicalDevice(), m_graphicsPipeline, nullptr);
        vkDestroyPipelineLayout(Device::GetSingleton()->GetVkLogicalDevice(), m_pipelineLayout, nullptr);
    }

    void GraphicsPipeline::AddLayoutBinding(uint32_t set, uint32_t binding, VkDescriptorType type, int descriptorCount, uint32_t stageFlags, const VkSampler* immutableSamplers)
    {
    	if (m_descriptorSetLayouts.size() <= set )
    	{
            m_descriptorSetLayouts.resize(set + 1);
    	}
    	
        m_descriptorSetLayouts[set].AddLayoutBinding(binding, type, descriptorCount, stageFlags, immutableSamplers);
    }

    void GraphicsPipeline::SetBindingDescription(uint32_t binding, uint32_t bindingSize, VkVertexInputRate vertexInputRate)
    {
        m_bindingDescription.binding = binding;
        m_bindingDescription.stride = bindingSize;
        m_bindingDescription.inputRate = vertexInputRate;
    }

    GraphicsPipeline::~GraphicsPipeline()
    {
        delete m_shaders.vertexShader;
        delete m_shaders.fragShader;
        delete m_shaders.geoShader;

        vkDestroyPipeline(Device::GetSingleton()->GetVkLogicalDevice(), m_graphicsPipeline, nullptr);
        vkDestroyPipelineLayout(Device::GetSingleton()->GetVkLogicalDevice(), m_pipelineLayout, nullptr);
    }

    void GraphicsPipeline::AddAttributeDescription(uint32_t binding, uint32_t location, VkFormat format, uint32_t offset)
    {
        VkVertexInputAttributeDescription attributeDescription;

        attributeDescription.binding = binding;
        attributeDescription.location = location;
        attributeDescription.format = format;
        attributeDescription.offset = offset;

        m_attributeDescriptions.push_back(attributeDescription);
    }

    void GraphicsPipeline::SetPrimitiveTopology(VkPrimitiveTopology primitiveTopology)
    {
        m_primitiveTopology = primitiveTopology;
    }

    VkPipeline GraphicsPipeline::GetVkPipeline() const
    {
        return m_graphicsPipeline;
    }

    VkPipelineLayout GraphicsPipeline::GetVkPipelineLayout() const
    {
        return m_pipelineLayout;
    }

    const DescriptorSetLayout& GraphicsPipeline::GetDescriptorLayout(uint32_t index) const
    {
        return m_descriptorSetLayouts[index];
    }

    int GraphicsPipeline::GetNbOfLayout() const
    {
        return m_nbOfLayout;
    }

    DescriptorSets& GraphicsPipeline::GetDescriptorSets()
    {
		return m_descriptorSets;
    }
}

