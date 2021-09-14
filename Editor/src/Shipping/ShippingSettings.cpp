#include "Shipping/ShippingSettings.h"

#include "Log/Logger.h"
#include <stdio.h>
#include <filesystem>

namespace NIX::Editor
{
	ShippingSettings::ShippingSettings(std::string_view buildFolder, EShippingMode shippingMode)
		: m_buildFolder(buildFolder), m_shippingMode(shippingMode)
	{
		UpdateScriptPath();
	}

	int ShippingSettings::GetSceneId(std::string_view sceneName)
	{
		int pos = -1;

		for (size_t i = 0; i < m_scenesInBuild.size(); i++)
		{
			if (m_scenesInBuild[i] == sceneName)
			{
				pos = (int)i;
				break;
			}
		}

		return pos;
	}

	const std::string_view ShippingSettings::GetVisualStudioFolder() const
	{
		return m_visualStudioFolder;
	}

	void ShippingSettings::SetVisualStudioFolder(std::string_view visualStudioFolder)
	{
		m_visualStudioFolder = visualStudioFolder;
	}

	const std::string_view ShippingSettings::GetBuildFolder() const
	{
		return m_buildFolder;
	}

	void ShippingSettings::SetBuildFolder(std::string_view buildFolder)
	{
		m_buildFolder = buildFolder.data();
	}

	const std::string_view ShippingSettings::GetBuildName() const
	{
		return m_buildName;
	}

	void ShippingSettings::SetBuildName(std::string_view buildName)
	{
		m_buildName = buildName.data();
	}

	EShippingMode ShippingSettings::GetShippingMode() const
	{
		return m_shippingMode;
	}

	void ShippingSettings::SetShippingMode(const EShippingMode shippingMode)
	{
		m_shippingMode = shippingMode;
		UpdateScriptPath();
	}

	void ShippingSettings::SetSceneName(std::string_view scene, std::string_view newName)
	{
		for (auto& sceneInBuild : m_scenesInBuild)
		{
			if (sceneInBuild == scene)
			{
				sceneInBuild = newName;
				break;
			}
		}
	}

	const std::vector<std::string>& ShippingSettings::GetScenesInBuild() const
	{
		return m_scenesInBuild;
	}

	bool ShippingSettings::AddSceneToBuild(const int id, std::string_view scenePath)
	{
		if (!std::filesystem::exists(scenePath))
		{
			return false;
		}
		
		
		if (id >= static_cast<int>(m_scenesInBuild.size()))
		{
			m_scenesInBuild.reserve(id);
		}
		
		const auto itScene = m_scenesInBuild.begin() + id;
		m_scenesInBuild.insert(itScene, scenePath.data());

		return true;
	}

	void ShippingSettings::RemoveSceneInBuild(int id)
	{
		m_scenesInBuild.erase(m_scenesInBuild.begin() + id);
	}

	void ShippingSettings::SwapSceneToBuild(const int firstId, const int secondId)
	{
		if (firstId > static_cast<int>(m_scenesInBuild.size()))
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_WARNING, "Scene with id : %d does not exist in the shipping settings", firstId);
			return;
		}
		if (secondId > static_cast<int>(m_scenesInBuild.size()))
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_WARNING, "Scene with id : %d does not exist in the shipping settings", secondId);
		}
		
		std::string buffer = m_scenesInBuild[firstId];

		m_scenesInBuild[firstId] = m_scenesInBuild[secondId];
		m_scenesInBuild[secondId] = buffer;
	}

	void ShippingSettings::CreateAndRunBuildingScript()
	{
		std::string scritpPath = "Resources\\Scripts\\shippingScript.bat";
		
		std::ofstream scriptFile;

		scriptFile.open(scritpPath);
		scriptFile << "call \"" << m_visualStudioFolder << "\\Community\\Common7\\Tools\\VsDevCmd.bat\"\n";
		scriptFile << "devenv \"..\\NIX_Engine.sln\" /Build " << m_buildTargetName << "\n";
		scriptFile << "ren \"..\\x64\\" << m_buildTargetName << "\\Editor.exe\" \"" << m_buildName << ".exe\"\n";
		scriptFile << "move \"..\\x64\\" << m_buildTargetName << "\" \"" << m_buildFolder << "\"\n";
		
		scriptFile.close();
		
		system(scritpPath.c_str());
	}

	void ShippingSettings::UpdateScriptPath()
	{
		switch (m_shippingMode)
		{
		case EShippingMode::DEBUG:
			m_buildTargetName = BUILD_TARGET_SHIPPING_DEBUG;
			break;
		case EShippingMode::RELEASE:
			m_buildTargetName = BUILD_TARGET_SHIPPING_RELEASE;
			break;
		}
	}
}
