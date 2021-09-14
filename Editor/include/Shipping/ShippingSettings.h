#pragma once

#define BUILD_TARGET_SHIPPING_DEBUG "Game_Shipping_Debug"
#define BUILD_TARGET_SHIPPING_RELEASE "Game_Shipping_Release"
#include <vector>
#include <string>

namespace NIX::Editor
{
	enum class EShippingMode
	{
		DEBUG = 0,
		RELEASE
	};
	
	class ShippingSettings
	{
	private:
		std::string m_visualStudioFolder = "%ProgramFiles(x86)%\\Microsoft Visual Studio\\2019";
		std::string m_buildFolder = "Builds";
		std::string m_buildName = "Build";
		EShippingMode m_shippingMode = EShippingMode::RELEASE;

		std::string m_buildTargetName = BUILD_TARGET_SHIPPING_RELEASE;

		inline static std::vector<std::string> m_scenesInBuild;
		
	public:
		ShippingSettings() = default;
		ShippingSettings(std::string_view buildFolder, EShippingMode shippingMode = EShippingMode::RELEASE);

		[[nodiscard]] static int GetSceneId(std::string_view sceneName);

		[[nodiscard]] const std::string_view GetVisualStudioFolder() const;
		void SetVisualStudioFolder(std::string_view visualStudioFolder);
		
		[[nodiscard]] const std::string_view GetBuildFolder() const;
		void SetBuildFolder(std::string_view buildFolder);

		[[nodiscard]] const std::string_view GetBuildName() const;
		void SetBuildName(std::string_view buildName);

		[[nodiscard]] EShippingMode GetShippingMode() const;

		void SetShippingMode(const EShippingMode shippingMode);

		static void SetSceneName(std::string_view scene, std::string_view newName);

		[[nodiscard]] const std::vector<std::string>& GetScenesInBuild() const;
		bool AddSceneToBuild(const int id, std::string_view scenePath);
		void RemoveSceneInBuild(int id);

		void SwapSceneToBuild(const int firstId, const int secondId);

		void CreateAndRunBuildingScript();
		
	private:
		void UpdateScriptPath();
		
		
	};
}
