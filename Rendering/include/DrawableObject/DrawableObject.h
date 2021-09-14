#pragma once

#include "API/DLLExportRendering.h"

#include "Memory/Buffer.h"
#include "Descriptors/DescriptorSets.h"
#include "Descriptors/DescriptorPool.h"	
#include "Material/Material.h"
#include "DrawableObject/RenderingMesh.h"

#include "Matrix/Matrix4.h"

#include <vulkan/vulkan_core.h>
#include <string>

namespace NIX::GeometricTools
{
	class Mesh;
}

namespace NIX::Rendering
{
	class GraphicsPipeline;
	class TextureView;

	class DrawableObject
	{
	private:
		RenderingMesh* m_renderingMesh;

		LibMath::Matrix4 m_transformation = LibMath::Matrix4::Identity();
		std::vector<bool> m_IsTransformationUpdate = { true, true, true };
		
		std::vector<Buffer> m_transformBuffer;

		bool m_isActive = true;
		Material* m_material = nullptr;
		DescriptorPool m_descriptorPool;
		DescriptorSets m_descriptorSets;
		GraphicsPipeline* m_graphicsPipeline = nullptr;

		CommandPool* m_commandPool = nullptr;
		
	public:
		DrawableObject() = delete;
		DrawableObject(const DrawableObject& other) = delete;
		DrawableObject(DrawableObject&& other) noexcept = delete;
		DrawableObject& operator=(const DrawableObject& other) = delete;
		DrawableObject& operator=(DrawableObject&& other) noexcept = delete;
		RENDERING_LIBRARY_API ~DrawableObject();

		DrawableObject(CommandPool& commandPool, GraphicsPipeline* graphicPipeline);
		DrawableObject(CommandPool& commandPool, const GeometricTools::Mesh& mesh, Material* material, GraphicsPipeline* graphicPipeline);
		DrawableObject(CommandPool& commandPool, const std::vector<LibMath::Vector3>& points, GraphicsPipeline* graphicPipeline);

		void CreateTransformBuffer(const CommandPool& commandPool);
		
		void SetAlbedo(TextureView* texture);
		void UpdateTransformationBuffer(uint32_t imageIndex);

		RENDERING_LIBRARY_API void ChangeVertex(const std::vector<LibMath::Vector3>& points);
		
		RENDERING_LIBRARY_API void UpdateTransformation(const LibMath::Matrix4& newTransform);
		RENDERING_LIBRARY_API void UpdateMaterial(Material* material);
		RENDERING_LIBRARY_API void UpdateRenderingMesh(GeometricTools::Mesh* mesh);
		RENDERING_LIBRARY_API void SetIsActive(bool isActive);

		void BindVertexBuffer(VkCommandBuffer commandBuffer);
		void BindIndexBuffer(VkCommandBuffer commandBuffer);
		void DrawVertex(VkCommandBuffer commandBuffer) const;
		void DrawIndex(VkCommandBuffer commandBuffer) const;

		[[nodiscard]] bool IsIndex() const;
		
		RENDERING_LIBRARY_API [[nodiscard]] RenderingMesh* GetRenderingMesh() const;
		[[nodiscard]] Material* GetMaterial() const;
		[[nodiscard]] GraphicsPipeline* GetPipeline() const;
		[[nodiscard]] const std::vector<Buffer>& GetTransformationBuffer() const;
		RENDERING_LIBRARY_API [[nodiscard]] bool GetIsActive() const;
		[[nodiscard]] DescriptorSets& GetDescriptorSets();
		[[nodiscard]] bool IsDescriptorSet();
		
		static VkVertexInputBindingDescription GetBindingDescription();

		static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();

	private:
		void SetupDescriptorPool();
	};
}
