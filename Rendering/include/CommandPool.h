#pragma once

#include <vulkan/vulkan_core.h>

namespace NIX::Rendering
{
	class Surface;

	class CommandPool
	{
	private:
		VkCommandPool m_commandPool;
	public:
		
		CommandPool() = default;
		CommandPool(const CommandPool& other) = delete;
		CommandPool(CommandPool&& other) noexcept = delete;
		CommandPool& operator=(const CommandPool& other) = delete;
		CommandPool& operator=(CommandPool&& other) noexcept = delete;
		~CommandPool();
		
		void Initialize(const Surface& surface);
		void Cleanup();

		[[nodiscard]] VkCommandPool GetCommandPool() const;

	};
}
