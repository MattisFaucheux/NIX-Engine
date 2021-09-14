#include "pch.h"

#include "DrawableObject/DrawableObject.h"

#include "Context/Context.h"
#include "MeshImport/Mesh.h"
#include "Memory/EBufferType.h"
#include "Material/TextureView.h"
#include "GraphicsPipeline.h"
#include "Log/Logger.h"
#include "Memory/Buffer.h"

namespace NIX::Rendering
{
	DrawableObject::DrawableObject(CommandPool& commandPool, GraphicsPipeline* graphicPipeline)
		: m_graphicsPipeline(graphicPipeline), m_commandPool(&commandPool)
	{
		m_graphicsPipeline = graphicPipeline;
		m_commandPool = &commandPool;

		SetupDescriptorPool();

		if (graphicPipeline == nullptr)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_WARNING, "Pipeline for this drawable object is null");
			return;
		}

		if (m_graphicsPipeline->GetNbOfLayout() >= 2)
		{
			m_descriptorSets.Initialize(m_graphicsPipeline->GetDescriptorLayout(2), m_descriptorPool);
		}

		CreateTransformBuffer(commandPool);

		

	}
	
	DrawableObject::DrawableObject(CommandPool& commandPool, const GeometricTools::Mesh& mesh, Material* material, GraphicsPipeline* graphicPipeline)
			: DrawableObject(commandPool, graphicPipeline)
	{
		m_renderingMesh = new RenderingMesh(commandPool, mesh);
		m_material = material;
	}

	DrawableObject::DrawableObject(CommandPool& commandPool, const std::vector<LibMath::Vector3>& m_points, GraphicsPipeline* graphicPipeline)
			: DrawableObject(commandPool, graphicPipeline)
	{
		m_renderingMesh = new RenderingMesh(commandPool, m_points);
	}

	DrawableObject::~DrawableObject()
	{
		delete m_renderingMesh;
	}

	

	void DrawableObject::CreateTransformBuffer(const CommandPool& commandPool)
	{
		m_transformBuffer.resize(MIN_IMAGE_COUNT);

		for (size_t i = 0; i < MIN_IMAGE_COUNT; i++)
		{
			m_transformBuffer[i].Initialize(commandPool, sizeof(MatrixModel), m_transformBuffer.data(), EBufferType::UNIFORM);
		}
	}

	void DrawableObject::SetAlbedo(TextureView* texture)
	{
		m_material->albedoMap = texture;
	}

	void DrawableObject::UpdateTransformationBuffer(uint32_t imageIndex)
	{
		if (!m_IsTransformationUpdate[imageIndex])
		{
			return;
		}
		
		MatrixModel modelMatrix;

		modelMatrix.model = m_transformation;

		m_transformBuffer[imageIndex].Update(sizeof(modelMatrix), &modelMatrix);

		if(IsDescriptorSet())
		{
			m_descriptorSets.SetIsUpdate(imageIndex, false);
		}
		
		m_IsTransformationUpdate[imageIndex] = false;
	}

	void DrawableObject::ChangeVertex(const std::vector<LibMath::Vector3>& points)
	{
		m_renderingMesh->ChangeVertex(*m_commandPool, points);
	}

	void DrawableObject::UpdateTransformation(const LibMath::Matrix4& newTransform)
	{
		m_transformation = newTransform;
		m_IsTransformationUpdate[0] = true;
		m_IsTransformationUpdate[1] = true;
		m_IsTransformationUpdate[2] = true;
	}

	void DrawableObject::UpdateMaterial(Material* material)
	{		
		m_material = material;
	}

	void DrawableObject::UpdateRenderingMesh(GeometricTools::Mesh* mesh)
	{
		if (mesh == nullptr)
		{
			return;
		}
		
		m_renderingMesh->Update(*m_commandPool, *mesh);
	}

	void DrawableObject::SetIsActive(bool isActive)
	{
		m_isActive = isActive;
	}

	void DrawableObject::BindVertexBuffer(VkCommandBuffer commandBuffer)
	{
		m_renderingMesh->vertexBuffer.BindVertexBuffer(commandBuffer);
	}

	void DrawableObject::BindIndexBuffer(VkCommandBuffer commandBuffer)
	{
		m_renderingMesh->indexBuffer.BindIndexBuffer(commandBuffer);
	}

	void DrawableObject::DrawVertex(VkCommandBuffer commandBuffer) const
	{
		vkCmdDraw(commandBuffer, static_cast<uint32_t>(m_renderingMesh->vertexCount), 1, 0, 0);
	}

	void DrawableObject::DrawIndex(VkCommandBuffer commandBuffer) const 
	{
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(m_renderingMesh->indexCount), 1, 0, 0, 0);
	}

	bool DrawableObject::IsIndex() const
	{
		if (m_renderingMesh != nullptr)
		{
			return m_renderingMesh->indexCount > 0;
		}
		
		return false;
	}

	Material* DrawableObject::GetMaterial() const
	{
		return m_material;
	}

	RenderingMesh* DrawableObject::GetRenderingMesh() const
	{
		return m_renderingMesh;
	}

	GraphicsPipeline* DrawableObject::GetPipeline() const
	{
		return m_graphicsPipeline;
	}

	const std::vector<Buffer>& DrawableObject::GetTransformationBuffer() const
	{
		return m_transformBuffer;
	}

	bool DrawableObject::GetIsActive() const
	{
		return m_isActive;
	}

	DescriptorSets& DrawableObject::GetDescriptorSets()
	{
		return m_descriptorSets;
	}

	bool DrawableObject::IsDescriptorSet()
	{
		return !m_descriptorSets.IsEmpty();
	}

	VkVertexInputBindingDescription DrawableObject::GetBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(GeometricTools::Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	std::vector<VkVertexInputAttributeDescription> DrawableObject::GetAttributeDescriptions()
	{
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
		attributeDescriptions.resize(3);

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(GeometricTools::Vertex, position);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(GeometricTools::Vertex, normal);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(GeometricTools::Vertex, texCord);

        return attributeDescriptions;
	}

	void DrawableObject::SetupDescriptorPool()
	{
		const std::vector<VkDescriptorPoolSize> poolSizes =
		{
			{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, MIN_IMAGE_COUNT * 2},
			{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, MIN_IMAGE_COUNT},
		};

		m_descriptorPool.Initialize(poolSizes, MIN_IMAGE_COUNT * 10);
	}
}
