#pragma once

#include <vulkan/vulkan_core.h>

#include <vector>

namespace NIX::Rendering
{
	class SwapChain;

	class SynchronizationObjects
	{
	private:
		std::vector<VkSemaphore> m_imageAvailableSemaphores;
		std::vector<VkSemaphore> m_renderFinishedSemaphores;
		std::vector<VkFence> m_inFlightFences;
		std::vector<VkFence> m_imagesInFlight;
		
	public:
		SynchronizationObjects() = default;
		SynchronizationObjects(const SynchronizationObjects& other) = delete;
		SynchronizationObjects(SynchronizationObjects&& other) noexcept = delete;
		SynchronizationObjects& operator=(const SynchronizationObjects& other) = delete;
		SynchronizationObjects& operator=(SynchronizationObjects&& other) noexcept = delete;
		~SynchronizationObjects();
		
		void Initialize();
		void Cleanup();

		[[nodiscard]] const VkFence& GetInFlightFence(uint32_t imageIndex) const;
		[[nodiscard]] const VkSemaphore& GetImageAvailableSemaphore(uint32_t imageIndex) const;
		
		[[nodiscard]] const VkFence& GetImageInFlight(uint32_t imageIndex) const;
		[[nodiscard]] VkFence& MutateImageInFlight(uint32_t imageIndex);

		[[nodiscard]] const VkSemaphore& GetRenderFinishedSemaphore(uint32_t imageIndex) const;
		
	};
}
