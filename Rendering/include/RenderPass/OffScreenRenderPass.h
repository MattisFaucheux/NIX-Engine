#pragma once
#include <vulkan/vulkan_core.h>

#include "Memory/Buffer.h"
#include "DrawableObject/DrawableObject.h"
#include "vector"
#include "Context/Device/Device.h"

#define OFF_SCREEN_IMAGE_FORMAT VK_FORMAT_R8G8B8A8_SRGB
#define OFF_SCREEN_DEFAULT_WIDTH 1920
#define OFF_SCREEN_DEFAULT_HEIGHT 1080

namespace NIX::Rendering
{
	class TextureSampler;
	class Camera;
	class GraphicsPipeline;
	class UIManager;
	class SwapChain;

	struct FrameBufferAttachment
	{
		VkImage image;
		VkDeviceMemory mem;
		VkImageView view;

		~FrameBufferAttachment();
	};
	
	class OffScreenRenderPass
	{
	private:
		int32_t m_width, m_height;
		float m_sceneWindowWidth, m_sceneWindowHeight;
		std::vector<VkFramebuffer> m_frameBuffers;
		std::vector<FrameBufferAttachment> m_colors;
		std::vector<bool> m_isTextureIdSet;
		FrameBufferAttachment m_depth;
		VkRenderPass m_renderPass;

		void BeginRenderPass(uint32_t imageIndex, VkCommandBuffer commandBuffer);
		void SetViewport(VkCommandBuffer commandBuffer);
		void SetScissor(VkCommandBuffer commandBuffer);
		
	public:
		OffScreenRenderPass() = default;
		OffScreenRenderPass(const OffScreenRenderPass& other) = delete;
		OffScreenRenderPass(OffScreenRenderPass&& other) noexcept = delete;
		OffScreenRenderPass& operator=(const OffScreenRenderPass& other) = delete;
		OffScreenRenderPass& operator=(OffScreenRenderPass&& other) noexcept = delete;
		~OffScreenRenderPass();

		void Initialize(const SwapChain& swapChain);

		void Cleanup();
		
		void Update(uint32_t imageIndex, VkCommandBuffer commandBuffer, UIManager& uiManager, std::vector<DrawableObject*>& drawableObjects,
					Camera& camera, const std::vector<Buffer>& lightBuffers, const std::vector<Buffer>& nbOfLightsBuffers,
					const TextureSampler& textureSampler/*, int width, int height*/);

		
		void BindDescriptorSetsByFrame(uint32_t imageIndex, VkCommandBuffer commandBuffer, Camera& camera, const std::vector<Buffer>& lightBuffers, const std::vector<Buffer>& nbOfLights, GraphicsPipeline& graphicsPipeline);
		void BindDescriptorSetsByMaterial(uint32_t imageIndex, VkCommandBuffer commandBuffer, const TextureSampler& textureSampler, Material* material, const GraphicsPipeline& graphicsPipeline);
		void BindDescriptorSetsByModel(uint32_t imageIndex, VkCommandBuffer commandBuffer, const std::vector<Buffer>& modelMatBuffers,  VkPipelineLayout pipelineLayout, DescriptorSets& descriptorSets);

		[[nodiscard]] VkRenderPass GetVkRenderPass() const;

		void UpdateSceneWindowSize(const float width, const float height);
		
	};
}
