#pragma once

#include "API/DLLExportRendering.h"

#include <vulkan/vulkan_core.h>

#include <vector>

namespace NIX::Rendering
{
	class DescriptorPool;
	class DescriptorSetLayout;

	class DescriptorSets
	{
	private:
		std::vector<VkDescriptorSet> m_descriptorSets;
		std::vector<bool> m_isUpdate;

		//DescriptorPool* m_descriptorPool = nullptr;
	public:
		DescriptorSets() = default;
		DescriptorSets(const DescriptorSets& other) = delete;
		DescriptorSets(DescriptorSets&& other) noexcept = delete;
		DescriptorSets& operator=(const DescriptorSets& other) = delete;
		DescriptorSets& operator=(DescriptorSets&& other) noexcept = delete;
		//RENDERING_LIBRARY_API ~DescriptorSets();
		
		~DescriptorSets() = default;
		
		void Initialize(const DescriptorSetLayout& descriptorSetLayout, DescriptorPool& descriptorPool);
		
		[[nodiscard]] bool IsUpdate(uint32_t imageIndex) const;
		void SetIsUpdate(uint32_t imageIndex, bool value);
		void SetAllIsUpdate(bool value);
		
		[[nodiscard]] bool IsEmpty() const;
		
		[[nodiscard]] VkDescriptorSet& GetVkDescriptorSet(uint32_t imageIndex);

	};
}
