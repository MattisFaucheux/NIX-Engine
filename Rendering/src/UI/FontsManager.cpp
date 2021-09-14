#include "pch.h"

#include "UI/FontsManager.h"

#include <vulkan/vulkan_core.h>


#include "ImGUI/imgui_impl_vulkan.h"
#include "Log/Logger.h"
#include "Memory/Buffer.h"

namespace NIX::Rendering
{
	FontsManager::FontsManager(CommandPool& commandPool)
		: m_commandPool(&commandPool)
	{}

	FontsManager::~FontsManager()
	{
        ImGui_ImplVulkan_DestroyFontsTexture();
	}

	void FontsManager::LoadDefaultFont(ImGuiIO& io)
	{		
        m_fonts.push_back({ {"Default Font", 13}, io.Fonts->AddFontFromFileTTF("Resources\\Fonts\\ProggyClean.ttf", 13.0f) });
	}

	void FontsManager::AddFontToLoad(const std::pair<std::string, int>& font)
	{
        m_fontsToLoad.emplace_back(font.first, font.second);
	}

	void FontsManager::UpdateFontToLoad()
	{
        if (!m_fontsToLoad.empty())
        {
            ImGuiIO& io = ImGui::GetIO();
            for (auto& font : m_fontsToLoad)
            {
                if (font.first == "Default Font")
                {
                    m_fonts.emplace_back(font, io.Fonts->AddFontFromFileTTF("Resources\\Fonts\\ProggyClean.ttf", (float)font.second));
                }
                else
                {
                    m_fonts.emplace_back(font, io.Fonts->AddFontFromFileTTF(font.first.c_str(), (float)font.second));
                }
            }

            m_fontsToLoad.clear();
            ImGui_ImplVulkan_DestroyFontsTexture();
        	
            VkCommandBuffer commandBuffer = Buffer::BeginSingleTimeCommands(*m_commandPool);
            if (!ImGui_ImplVulkan_CreateFontsTexture(commandBuffer))
            {
                ASSERT(SOURCE_INFO, "Failed to create ImGui font textures !");
            }
            Buffer::EndSingleTimeCommands(*m_commandPool, commandBuffer);

            ImGui_ImplVulkan_DestroyFontUploadObjects();
        }
	}

	const std::vector<std::pair<std::pair<std::string, int>, ImFont*>>& FontsManager::GetFonts() const
	{
        return m_fonts;
	}
}
