#pragma once

#include <vulkan/vulkan_core.h>

#include "Material/TextureView.h"
#include "RenderPass/RenderPass.h"
#include "Material/Texture.h"

#include <vector>

const uint32_t MIN_IMAGE_COUNT = 3;
const int MAX_FRAMES_IN_FLIGHT = 2;

namespace NIX::Rendering
{
	class RenderPass;
	class Surface;
	
	class SwapChain
	{
	private:
		VkSwapchainKHR m_swapChain;
		VkFormat m_swapChainImageFormat;
		VkExtent2D m_swapChainExtent;
		
		std::vector<VkImage> m_swapChainImages;
		
		std::vector<TextureView> m_imageViews;
		
		std::vector<VkFramebuffer> m_swapChainFrameBuffers;

		Texture m_depthImage;
		TextureView m_depthImageView;
	public:
		SwapChain() = default;
		SwapChain(const SwapChain& other) = delete;
		SwapChain(SwapChain&& other) noexcept = delete;
		SwapChain& operator=(const SwapChain& other) = delete;
		SwapChain& operator=(SwapChain&& other) noexcept = delete;
		~SwapChain();
		
		void Initialize(const Surface& surface);
		
		[[nodiscard]] VkSwapchainKHR GetSwapChain() const;
		[[nodiscard]] const VkFormat& GetImageFormat() const;
		[[nodiscard]] const VkExtent2D& GetExtent() const;
		[[nodiscard]] const std::vector<VkFramebuffer>& GetFrameBuffers() const;
		[[nodiscard]] const std::vector<VkImage>& GetSwapChainImages() const;

		
		void CreateFrameBuffers(const RenderPass& renderPass);

		void Cleanup();
		void CleanupImageViews();
		void Destroy();
	
	private:
		void CreateDepthImageView();
		void CreateImageViews();
		
		[[nodiscard]] VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		[[nodiscard]] VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		[[nodiscard]] VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	};
}
