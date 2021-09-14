#include "ProjectSave.h"


#include "EditorApp.h"
#include "SceneManager.h"
#include "Serialization/SceneSave/Serializer.h"
#include "UI/InspectorWindow.h"
#include "UI/UIEditor.h"

namespace NIX::Editor
{
	void ProjectSave::Serialize()
	{
		std::string projectPath = "test";
		
		Core::Serializer serializer;

		Core::SaveElement rootElement = serializer.AddRootElement("project_save");

		SerializeShippingSettings(rootElement);
		
		SerializeTags(rootElement);

		serializer.Serialize(PROJECT_SAVE_PATH, rootElement);
	}

	void ProjectSave::Deserialize()
	{
		Core::Serializer serializer;

		serializer.Deserialize(PROJECT_SAVE_PATH);

		std::pair<bool, Core::SaveElement> rootElement = serializer.GetFirstElement("project_save");
		if(rootElement.first)
		{
			DeserializeShippingSettings(rootElement.second);
			DeserializeTags(rootElement.second);
		}
	}

	void ProjectSave::SerializeShippingSettings(Core::SaveElement& parent)
	{
		ShippingSettings& shippingSettings = EditorApp::GetShippingSettings();
		
		Core::SaveElement shippingSettingsElement(parent, "shipping_settings");

		shippingSettingsElement.AddStringAttribute("visual_studio_folder", shippingSettings.GetVisualStudioFolder().data());
		shippingSettingsElement.AddStringAttribute("build_folder", shippingSettings.GetBuildFolder().data());
		shippingSettingsElement.AddStringAttribute("build_name", shippingSettings.GetBuildName().data());
		shippingSettingsElement.AddIntAttribute("shipping_mode", static_cast<int>(shippingSettings.GetShippingMode()));

		shippingSettingsElement.AddStringAttribute("scene_to_load", SceneManager::GetScenePath().data());
		
		Core::SaveElement scenesInBuildElem(shippingSettingsElement, "scenes_in_build");
		
		for (const auto& scene : shippingSettings.GetScenesInBuild())
		{
			Core::SaveElement sceneElement(scenesInBuildElem, "scene");
			sceneElement.AddStringAttribute("path", scene.c_str());			
		}
	}

	void ProjectSave::DeserializeShippingSettings(Core::SaveElement& parent)
	{
		ShippingSettings& shippingSettings = EditorApp::GetShippingSettings();
		
		std::pair<bool, Core::SaveElement> shippingSettingsElem = parent.GetChild("shipping_settings");
		if (shippingSettingsElem.first)
		{
			shippingSettings.SetVisualStudioFolder(shippingSettingsElem.second.GetStringAttribute("visual_studio_folder"));
			shippingSettings.SetBuildFolder(shippingSettingsElem.second.GetStringAttribute("build_folder"));
			shippingSettings.SetBuildName(shippingSettingsElem.second.GetStringAttribute("build_name"));
			shippingSettings.SetShippingMode(static_cast<EShippingMode>(shippingSettingsElem.second.GetIntAttribute("shipping_mode")));

			const std::pair<bool, Core::SaveElement> sceneInBuildElem = shippingSettingsElem.second.GetChild("scenes_in_build");
		
			std::pair<bool, Core::SaveElement> sceneElem = sceneInBuildElem.second.GetChild("scene");
			int id = 0;
			while (sceneElem.first)
			{
				if (shippingSettings.AddSceneToBuild(id, sceneElem.second.GetStringAttribute("path")))
				{
					id++;
				}
				
				sceneElem = sceneElem.second.GetNextSibling();
			}

#ifdef _SHIPPING
			SceneManager::SetScenePath(shippingSettings.GetScenesInBuild()[0]);
#endif
#ifndef _SHIPPING
			SceneManager::SetScenePath(shippingSettingsElem.second.GetStringAttribute("scene_to_load"));
#endif
		}
			
	}

	void ProjectSave::SerializeTags(Core::SaveElement& parent)
	{
		const std::vector<std::string>& tagsList = UIEditor::GetInspectorWindow().GetTagsList();
		
		Core::SaveElement tagsElement(parent, "tags");

		for (const auto& tag : tagsList)
		{
			Core::SaveElement tagElem(tagsElement, "tag");
			tagElem.AddStringAttribute("name", tag.data());
		}
	}

	void ProjectSave::DeserializeTags(Core::SaveElement& parent)
	{		
		std::pair<bool, Core::SaveElement> tagsElem = parent.GetChild("tags");
		if (tagsElem.first)
		{
			std::pair<bool, Core::SaveElement> tag = tagsElem.second.GetChild("tag");
						
			while (tag.first)
			{
				UIEditor::GetInspectorWindow().AddTag(tag.second.GetStringAttribute("name"));
				tag = tag.second.GetNextSibling();
			}
			
		}

		UIEditor::GetInspectorWindow().SetTagsNb();
	}
}
