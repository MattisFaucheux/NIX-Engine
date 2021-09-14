#pragma once


#include <vulkan/vulkan_core.h>

#include <vector>

namespace NIX::Rendering
{
	class Instance
	{
	private:
		VkInstance m_instance;
	public:
		Instance() = default;
		~Instance();
		
		void Initialize(const VkApplicationInfo& appInfo, std::vector<const char*>& requiredExtension);
		
		
		[[nodiscard]] VkInstance GetVkInstance() const;
	};
}
