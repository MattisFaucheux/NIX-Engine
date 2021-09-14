#pragma once

#include "API/DLLExportRendering.h"

#include <vulkan/vulkan_core.h>

#include <vector>

namespace NIX::Rendering
{
	class LogicalDevice;
	class SwapChain;

	class DescriptorPool
	{
	private:
		VkDescriptorPool m_descriptorPool;
		
	public:
		DescriptorPool() = default;
		DescriptorPool(const DescriptorPool& other) = delete;
		DescriptorPool(DescriptorPool&& other) noexcept = delete;
		DescriptorPool& operator=(const DescriptorPool& other) = delete;
		DescriptorPool& operator=(DescriptorPool&& other) noexcept = delete;
		RENDERING_LIBRARY_API ~DescriptorPool();
		
		void Initialize(const std::vector<VkDescriptorPoolSize>& poolSizes, uint32_t maxSets);
		void Cleanup();

		[[nodiscard]] VkDescriptorPool GetVkDescriptorPool() const;

	};
}
