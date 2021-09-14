#pragma once
#include <string>
#include "API/DLLExportCore.h"
#include "tinyxml2.h"

#include "Vector/Vector3.h"

namespace NIX::Core
{
	class XMLParser
	{		
	public:
		CORE_LIBRARY_API static void LoadXMLDocument(const std::string& filePath);

		CORE_LIBRARY_API void CreateXMLDocument(const std::string& filePath);

	private:
		tinyxml2::XMLElement* AddSceneElement(tinyxml2::XMLDocument& xmlDoc, const int id, const std::string& sceneName);

		void AddGameObject(tinyxml2::XMLElement* parent, const std::string& gameObjectName);

		void AddTransformComponent(tinyxml2::XMLElement* parent, const LibMath::Vector3& position, const LibMath::Vector3& rotation, const LibMath::Vector3& scale);

		void AddVec3Element(tinyxml2::XMLElement* parent, const std::string& name, const LibMath::Vector3& value);
		
	};
}
