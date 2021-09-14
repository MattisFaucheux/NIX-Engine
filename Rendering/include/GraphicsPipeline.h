#pragma once

#include "Descriptors/DescriptorSetLayout.h"
#include "Descriptors/DescriptorSets.h"
#include "Descriptors/DescriptorPool.h"

#include <vulkan/vulkan_core.h>

namespace NIX::Rendering
{
	class Shader;
	class DescriptorSetLayout;
	class LogicalDevice;
	class RenderPass;
	class SwapChain;

	class GraphicsPipeline
	{
	private:
		VkPipelineLayout m_pipelineLayout = nullptr;
		VkPipeline m_graphicsPipeline = nullptr;
		std::vector<DescriptorSetLayout> m_descriptorSetLayouts;
		
		DescriptorPool m_descriptorPool;
		
		struct Shaders
		{
			Shader* vertexShader = nullptr;
			Shader* fragShader = nullptr;
			Shader* geoShader = nullptr;
		} m_shaders;

		VkVertexInputBindingDescription m_bindingDescription;
		std::vector<VkVertexInputAttributeDescription> m_attributeDescriptions;

		VkPrimitiveTopology m_primitiveTopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

		DescriptorSets m_descriptorSets;
		int m_nbOfLayout = 0;
		
	public:
		
		GraphicsPipeline() = default;
		GraphicsPipeline(const GraphicsPipeline& other) = delete;
		GraphicsPipeline(GraphicsPipeline&& other) noexcept = delete;
		GraphicsPipeline& operator=(const GraphicsPipeline& other) = delete;
		GraphicsPipeline& operator=(GraphicsPipeline&& other) noexcept = delete;
		~GraphicsPipeline();

		void Initialize(const char* vertexShader, const char* fragShader, const char* geoShader, const SwapChain& swapChain, VkRenderPass renderPass);
		void InitializeDescriptorSetLayout();

		void Cleanup();
		
		void AddLayoutBinding(uint32_t set, uint32_t binding, VkDescriptorType type, int descriptorCount, uint32_t stageFlags, const VkSampler* immutableSamplers = nullptr);
		void SetBindingDescription(uint32_t binding, uint32_t bindingSize, VkVertexInputRate vertexInputRate);

		void AddAttributeDescription(uint32_t binding, uint32_t location, VkFormat format, uint32_t offset);

		void SetPrimitiveTopology(VkPrimitiveTopology primitiveTopology);
		
		[[nodiscard]] VkPipeline GetVkPipeline() const;
		[[nodiscard]] VkPipelineLayout GetVkPipelineLayout() const;
		[[nodiscard]] const DescriptorSetLayout& GetDescriptorLayout(uint32_t index) const;
		[[nodiscard]] int GetNbOfLayout() const;
		[[nodiscard]] DescriptorSets& GetDescriptorSets();

	private:
		void InitializeShaders(const char* vertexShader, const char* fragShader, const char* geoShader);
	};
}
