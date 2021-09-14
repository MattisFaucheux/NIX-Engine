#pragma once
#include <vulkan/vulkan_core.h>

#include <vector>

namespace NIX::Rendering
{
	class CommandPool;
	class SwapChain;

	class CommandBuffers
	{
	private:
		std::vector<VkCommandBuffer> m_commandBuffers;
		CommandPool* m_commandPool = nullptr;

	public:
		CommandBuffers() = default;
		CommandBuffers(const CommandBuffers& other) = delete;
		CommandBuffers(CommandBuffers&& other) noexcept = delete;
		CommandBuffers& operator=(const CommandBuffers& other) = delete;
		CommandBuffers& operator=(CommandBuffers&& other) noexcept = delete;
		~CommandBuffers();

		void Initialize(CommandPool& commandPool);
		void Cleanup();

		void StartUpdate(uint32_t imageIndex);

		void EndUpdate(uint32_t imageIndex);
				
		[[nodiscard]] VkCommandBuffer GetVkCommandBuffer(uint32_t imageIndex) const;
		[[nodiscard]] VkCommandBuffer& GetPtrCommandBuffer(uint32_t imageIndex);
		
	};
}
