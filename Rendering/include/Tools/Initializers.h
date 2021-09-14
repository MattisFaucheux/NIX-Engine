#pragma once

#include <vulkan/vulkan_core.h>

#include <string>
#include <vector>

namespace NIX::Rendering
{
	class CommandPool;
	struct Version;

	struct Initializers
	{
		[[nodiscard]] static VkApplicationInfo ApplicationInfo(const std::string& appName, const Version& appVersion, const std::string& engineName, const Version& engineVersion);

		[[nodiscard]] static VkInstanceCreateInfo InstanceCreateInfo(const VkApplicationInfo& appInfo);

		[[nodiscard]] static VkSubmitInfo SubmitInfo();

		[[nodiscard]] static VkPresentInfoKHR PresentInfo();

		[[nodiscard]] static VkCommandPoolCreateInfo CommandPoolCreateInfo();
		
		[[nodiscard]] static VkCommandBufferAllocateInfo CommandBufferAllocateInfo(const CommandPool& commandPool, VkCommandBufferLevel level, uint32_t bufferCount);

		[[nodiscard]] static VkCommandBufferBeginInfo CommandBufferBeginInfo();
		
		[[nodiscard]] static VkRenderPassBeginInfo RenderPassBeginInfo();

		[[nodiscard]] static VkRenderPassCreateInfo RenderPassCreateInfo();

		[[nodiscard]] static VkImageCreateInfo ImageCreateInfo();
		
		[[nodiscard]] static VkImageViewCreateInfo ImageViewCreateInfo();

		[[nodiscard]] static VkImageMemoryBarrier ImageMemoryBarrier();

		[[nodiscard]] static VkDescriptorSetAllocateInfo DescriptorSetAllocateInfo(VkDescriptorPool descriptorPool, const VkDescriptorSetLayout* pSetLayouts, uint32_t descriptorSetCount);

		[[nodiscard]] static VkDescriptorPoolCreateInfo DescriptorPoolCreateInfo(const std::vector<VkDescriptorPoolSize>& poolSizes, uint32_t maxSets);

		[[nodiscard]] static VkDescriptorSetLayoutCreateInfo DescriptorSetLayoutCreateInfo();
		
		[[nodiscard]] static VkMemoryAllocateInfo MemoryAllocateInfo();

		[[nodiscard]] static VkSamplerCreateInfo SamplerCreateInfo();

		[[nodiscard]] static VkFramebufferCreateInfo FrameBufferCreateInfo();

		[[nodiscard]] static VkBufferCreateInfo BufferCreateInfo(VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharingMode);

		[[nodiscard]] static VkViewport Viewport(float width, float height, float minDepth, float maxDepth);

		[[nodiscard]] static VkRect2D Rect2D(int32_t width, int32_t height, int32_t offsetX, int32_t offsetY);

		[[nodiscard]] static VkShaderModuleCreateInfo ShaderModuleCreateInfo(size_t codeSize);

		[[nodiscard]] static VkDeviceQueueCreateInfo DeviceQueueCreateInfo();

		[[nodiscard]] static VkDeviceCreateInfo DeviceCreateInfo();

		[[nodiscard]] static VkSwapchainCreateInfoKHR SwapChainCreateInfoKHR(VkSurfaceKHR surface);

		[[nodiscard]] static VkPipelineShaderStageCreateInfo PipelineShaderStageCreateInfo();

		[[nodiscard]] static VkPipelineVertexInputStateCreateInfo PipelineVertexInputStateCreateInfo();

		[[nodiscard]] static VkPipelineInputAssemblyStateCreateInfo PipelineInputAssemblyStateCreateInfo();

		[[nodiscard]] static VkPipelineViewportStateCreateInfo PipelineViewportStateCreateInfo();

		[[nodiscard]] static VkPipelineRasterizationStateCreateInfo PipelineRasterizationStateCreateInfo();

		[[nodiscard]] static VkPipelineMultisampleStateCreateInfo PipelineMultisampleStateCreateInfo();

		[[nodiscard]] static VkPipelineDepthStencilStateCreateInfo PipelineDepthStencilStateCreateInfo();
		
		[[nodiscard]] static VkPipelineColorBlendStateCreateInfo PipelineColorBlendStateCreateInfo();

		[[nodiscard]] static VkPipelineLayoutCreateInfo PipelineLayoutCreateInfo();

		[[nodiscard]] static VkPipelineDynamicStateCreateInfo PipelineDynamicStateCreateInfo();

		[[nodiscard]] static VkGraphicsPipelineCreateInfo GraphicsPipelineCreateInfo();
	};
}
