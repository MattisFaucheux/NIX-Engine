#pragma once
#include <vulkan/vulkan_core.h>

#include <vector>
#include <string>

struct ImDrawData;

struct ImFont;

namespace NIX::Rendering
{
	class TextureSampler;
	class TextureView;
	class CommandPool;
	class RenderPass;
	class SwapChain;
	class FontsManager;

	class UIManager
	{
	private:
		VkDescriptorPool m_descriptorPool;

		std::vector<void*> m_textureIds;	
				
		TextureSampler* m_textureSampler;

		inline static FontsManager* m_fontManagers = nullptr;

		static ImFont* CheckFont(std::pair<std::string, int>& font);

	public:
		
		UIManager() = default;
		UIManager(const UIManager& other) = delete;
		UIManager(UIManager&& other) noexcept = delete;
		UIManager& operator=(const UIManager& other) = delete;
		UIManager& operator=(UIManager&& other) noexcept = delete;
		~UIManager();

		std::vector<void*>& GetTextureIds();

		void InitImGUI(const VkInstance vkInstance, const SwapChain& swapChain, CommandPool& commandPool, const RenderPass& renderPass, TextureSampler* textureSampler);
		
		void SetTextureIdScene(uint32_t imageIndex, VkImageView imageView);
		
		void PrepareNewFrame();
		
		void RenderDrawData(VkCommandBuffer commandBuffer) const;

		static void AddFontToLoad(std::pair<std::string, int> font);
		static ImFont* GetFont(std::pair<std::string, int>& font);
	};
}
