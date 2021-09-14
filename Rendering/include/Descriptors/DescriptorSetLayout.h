#pragma once

#include "API/DLLExportRendering.h"

#include <vulkan/vulkan_core.h>

#include <vector>

namespace NIX::Rendering
{
	class DescriptorSetLayout
	{
	private:
		VkDescriptorSetLayout m_descriptorSetLayout = nullptr;
		std::vector<VkDescriptorSetLayoutBinding> m_layoutBindings;
	public:

		DescriptorSetLayout() = default;
		DescriptorSetLayout(const DescriptorSetLayout& other) = delete;
		DescriptorSetLayout(DescriptorSetLayout&& other) noexcept = default;
		DescriptorSetLayout& operator=(const DescriptorSetLayout& other) = delete;
		DescriptorSetLayout& operator=(DescriptorSetLayout&& other) noexcept = delete;
		RENDERING_LIBRARY_API ~DescriptorSetLayout();
		
		void Initialize();
		void Cleanup();

		void AddLayoutBinding(int binding, VkDescriptorType type, int descriptorCount, uint32_t stageFlags, const VkSampler* immutableSamplers = nullptr);
		
		[[nodiscard]] VkDescriptorSetLayout GetVkDescriptorSetLayout() const;
	};
}
