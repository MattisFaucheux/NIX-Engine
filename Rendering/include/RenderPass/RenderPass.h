#pragma once
#include <vulkan/vulkan_core.h>

namespace NIX::Rendering
{
	class UIManager;
	class GraphicsPipeline;
	class SwapChain;

	class RenderPass
	{
	public:
		RenderPass() = default;
		RenderPass(const RenderPass& other) = delete;
		RenderPass(RenderPass&& other) noexcept = delete;
		RenderPass& operator=(const RenderPass& other) = delete;
		RenderPass& operator=(RenderPass&& other) noexcept = delete;
		~RenderPass();

		void Initialize(const SwapChain& swapChain);

		void Update(uint32_t imageIndex, VkCommandBuffer commandBuffer, const SwapChain& swapChain, const GraphicsPipeline& graphicsPipeline, UIManager& uiManager);
		
		void Cleanup();

		[[nodiscard]] VkRenderPass GetVkRenderPass() const;
	private:
		VkRenderPass m_renderPass;
	};
}
