#include "pch.h"

#include "External/tinyxml/XMLParser.h"

#include <string>


#include "External/tinyxml/tinyxml2.h"
#include "Log/Logger.h"

using namespace tinyxml2;

namespace NIX::Core
{
	void XMLParser::LoadXMLDocument(const std::string& filePath)
	{
		XMLDocument doc;
		doc.LoadFile(filePath.c_str());
	}

	void XMLParser::CreateXMLDocument(const std::string& filePath)
	{
		XMLDocument xmlDoc;
		
		
		xmlDoc.Clear();
				
		// Add Scene

		XMLElement* root = AddSceneElement(xmlDoc, 0, "sceneTest");
		
		xmlDoc.InsertFirstChild(root);

		//AddGameObject(root, "Cube");
		//AddGameObject(root, "Plant");
		
		const XMLError xmlError = xmlDoc.SaveFile(filePath.c_str());
		if (xmlError != XML_SUCCESS)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_WARNING, "Failed to save xml file : %s", filePath.c_str());
		}
	}

	XMLElement* XMLParser::AddSceneElement(XMLDocument& xmlDoc, const int id, const std::string& sceneName)
	{
		XMLElement* sceneElement = xmlDoc.NewElement("scene");

		sceneElement->SetAttribute("id", id);
		sceneElement->SetAttribute("name", sceneName.c_str());

		return sceneElement;
	}

	void XMLParser::AddGameObject(XMLElement* parent, const std::string& gameObjectName)
	{
		XMLElement* gameObject = parent->InsertNewChildElement("game_object");
		gameObject->SetAttribute("name", "TestGameObject");
		AddTransformComponent(gameObject, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });
	}

	void XMLParser::AddTransformComponent(tinyxml2::XMLElement* parent, const LibMath::Vector3& position, const LibMath::Vector3& rotation, const LibMath::Vector3& scale)
	{
		XMLElement* transformComp = parent->InsertNewChildElement("transform_component");

		AddVec3Element(transformComp, "position", position);
		AddVec3Element(transformComp, "rotation", rotation);
		AddVec3Element(transformComp, "scale", scale);
		
	}

	void XMLParser::AddVec3Element(tinyxml2::XMLElement* parent, const std::string& name, const LibMath::Vector3& value)
	{
		XMLElement* vectorElem = parent->InsertNewChildElement(name.c_str());
		vectorElem->SetAttribute("x", value.x);
		vectorElem->SetAttribute("y", value.y);
		vectorElem->SetAttribute("z", value.z);
	}
}
