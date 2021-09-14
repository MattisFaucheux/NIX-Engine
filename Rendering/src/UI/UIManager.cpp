#include "pch.h"

#include "UI/UIManager.h"

#include <stdexcept>


#include "Memory/Buffer.h"
#include "Context/Device/Device.h"
#include "SwapChain/SwapChain.h"
#include "CommandPool.h"
#include "Context/Context.h"
#include "RenderPass/RenderPass.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_vulkan.h"
#include "Log/Logger.h"

#include "UI/UI.h"

#include "Material/TextureSampler.h"
#include "UI/FontsManager.h"

namespace NIX::Rendering
{
    ImFont* UIManager::CheckFont(std::pair<std::string, int>& font)
    {
        for (const auto& fontIt : m_fontManagers->GetFonts())
        {
            if (fontIt.first == font)
            {
                return fontIt.second;
            }
        }

        return nullptr;
    }

    UIManager::~UIManager()
	{    	
        vkDestroyDescriptorPool(Device::GetSingleton()->GetVkLogicalDevice(), m_descriptorPool, nullptr);
        delete m_fontManagers;
    	
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
	}

    std::vector<void*>& UIManager::GetTextureIds()
    {
        return m_textureIds;
    }

    void UIManager::InitImGUI(const VkInstance vkInstance, const SwapChain& swapChain, CommandPool& commandPool, const RenderPass& renderPass, TextureSampler* textureSampler)
	{
        m_fontManagers = new FontsManager(commandPool);
    	if (m_fontManagers == nullptr)
    	{
            ASSERT(SOURCE_INFO, "Fonts manager creation failed");
    	}
    	
        VkDescriptorPoolSize pool_sizes[] =
        {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
        };

        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1000;
        pool_info.poolSizeCount = static_cast<uint32_t>(std::size(pool_sizes));
        pool_info.pPoolSizes = pool_sizes;

        if (vkCreateDescriptorPool(Device::GetSingleton()->GetVkLogicalDevice(), &pool_info, nullptr, &m_descriptorPool) != VK_SUCCESS)
        {
            ASSERT(SOURCE_INFO, "Failed to create descriptor pool for Imgui !");
        }

        // 2: initialize imgui library

        //this initializes the core structures of imgui
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(static_cast<float>(swapChain.GetExtent().width), static_cast<float>(swapChain.GetExtent().height));
        m_fontManagers->LoadDefaultFont(io);
    	

        //this initializes imgui for GLFW
        ImGui_ImplGlfw_InitForVulkan(Device::GetSingleton()->GetWindow()->GetWindow(), true);

        //this initializes imgui for Vulkan
        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = vkInstance;
        init_info.PhysicalDevice = Device::GetSingleton()->GetVkPhysicalDevice();
        init_info.Device = Device::GetSingleton()->GetVkLogicalDevice();
        init_info.Queue = Device::GetSingleton()->GetGraphicsQueue();
        init_info.DescriptorPool = m_descriptorPool;
        init_info.MinImageCount = 3;
        init_info.ImageCount = 3;

        ImGui_ImplVulkan_Init(&init_info, renderPass.GetVkRenderPass());

        VkCommandBuffer commandBuffer = Buffer::BeginSingleTimeCommands(commandPool);
		if (!ImGui_ImplVulkan_CreateFontsTexture(commandBuffer))
		{
            ASSERT(SOURCE_INFO, "Failed to create ImGui font textures !");
		}
        Buffer::EndSingleTimeCommands(commandPool, commandBuffer);

        ImGui_ImplVulkan_DestroyFontUploadObjects();

        m_textureSampler = textureSampler;

        m_textureIds.resize(swapChain.GetSwapChainImages().size());

#ifdef _SHIPPING
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowBorderSize = 0.0f;
        style.WindowPadding = { 0, 0 };

#endif
      
        UI::GetUI()->InitDocking();

	}

	void UIManager::SetTextureIdScene(uint32_t imageIndex, VkImageView imageView)
	{
        if (m_textureSampler != nullptr)
        {
        	m_textureIds[imageIndex] = static_cast<void*>(ImGui_ImplVulkan_AddTexture(m_textureSampler->GetVkTextureSampler(), imageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));
        }
	}

	void UIManager::PrepareNewFrame()
	{
        m_fontManagers->UpdateFontToLoad();

        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();
	}

	void UIManager::RenderDrawData(VkCommandBuffer commandBuffer) const
	{
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
	}

    void UIManager::AddFontToLoad(std::pair<std::string, int> font)
    {
        if (!CheckFont(font))
        {
            m_fontManagers->AddFontToLoad(font);
        }
    }

    ImFont* UIManager::GetFont(std::pair<std::string, int>& font)
    {
        return CheckFont(font);
    }
}
