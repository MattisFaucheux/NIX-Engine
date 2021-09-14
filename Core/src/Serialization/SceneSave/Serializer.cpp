#include "pch.h"

#include "Serialization/SceneSave/Serializer.h"
#include "External/tinyxml/tinyxml2.h"

#include "Vector/Vector3.h"
#include "Vector/Vector4.h"
#include "Quaternion/Quaternion.h"

#include <tuple>

#include "Log/Logger.h"

namespace NIX::Core
{
	SaveElement::SaveElement(tinyxml2::XMLDocument& xmlDoc, const char* elementName)
	{
		element = xmlDoc.NewElement(elementName);
	}

	SaveElement::SaveElement(SaveElement& parent, const std::string& name)
	{
		element = parent.element->InsertNewChildElement(name.c_str());
	}

	void SaveElement::AddIntAttribute(const char* name, const int value)
	{
		element->SetAttribute(name, value);
	}

	void SaveElement::AddFloatAttribute(const char* name, const float value)
	{
		element->SetAttribute(name, value);
	}

	void SaveElement::AddStringAttribute(const char* name, const char* value)
	{
		element->SetAttribute(name, value);
	}

	void SaveElement::AddBoolAttribute(const char* name, const bool value)
	{
		element->SetAttribute(name, value);
	}

	void SaveElement::AddVec3Element(const std::string& name, const LibMath::Vector3& value)
	{
		tinyxml2::XMLElement* vectorElem = element->InsertNewChildElement(name.c_str());
		vectorElem->SetAttribute(ATTRIBUTE_VECTOR_X, value.x);
		vectorElem->SetAttribute(ATTRIBUTE_VECTOR_Y, value.y);
		vectorElem->SetAttribute(ATTRIBUTE_VECTOR_Z, value.z);
	}

	void SaveElement::AddVec4Element(const std::string& name, const LibMath::Quaternion& value)
	{
		tinyxml2::XMLElement* vectorElem = element->InsertNewChildElement(name.c_str());
		vectorElem->SetAttribute(ATTRIBUTE_VECTOR_X, value.X);
		vectorElem->SetAttribute(ATTRIBUTE_VECTOR_Y, value.Y);
		vectorElem->SetAttribute(ATTRIBUTE_VECTOR_Z, value.Z);
		vectorElem->SetAttribute(ATTRIBUTE_VECTOR_W, value.W);
	}

	void SaveElement::AddVec4Element(const std::string& name, const LibMath::Vector4& value)
	{
		tinyxml2::XMLElement* vectorElem = element->InsertNewChildElement(name.c_str());
		vectorElem->SetAttribute(ATTRIBUTE_VECTOR_X, value.x);
		vectorElem->SetAttribute(ATTRIBUTE_VECTOR_Y, value.y);
		vectorElem->SetAttribute(ATTRIBUTE_VECTOR_Z, value.z);
		vectorElem->SetAttribute(ATTRIBUTE_VECTOR_W, value.w);
	}

	void SaveElement::AddVec3BoolElement(const std::string& name, const std::tuple<bool, bool, bool>& value)
	{
		AddVec3BoolElement(name, std::get<0>(value), std::get<1>(value), std::get<2>(value));
	}

	void SaveElement::AddVec3BoolElement(const std::string& name, bool x, bool y, bool z)
	{
		tinyxml2::XMLElement* vectorElem = element->InsertNewChildElement(name.c_str());
		vectorElem->SetAttribute(ATTRIBUTE_VECTOR_X, x);
		vectorElem->SetAttribute(ATTRIBUTE_VECTOR_Y, y);
		vectorElem->SetAttribute(ATTRIBUTE_VECTOR_Z, z);
	}

	std::pair<bool, SaveElement> SaveElement::GetFirstChild() const
	{
		SaveElement firstChild;

		firstChild.element = element->FirstChildElement();

		if (firstChild.element == nullptr)
		{
			LOG(SOURCE_INFO, EVerbosity::LOG_WARNING, "No first child found in xml file");
			return { false, firstChild };
		}
		
		return { true, firstChild };
	}

	std::pair<bool, SaveElement> SaveElement::GetChild(const char* name) const
	{
		SaveElement child;
		child.element = element->FirstChildElement(name);

		if (child.element == nullptr)
		{
			LOG(SOURCE_INFO, EVerbosity::LOG_WARNING, "No child found with the name : %s, in xml file", name);
			return { false, child };
		}
		
		return {true, child };
	}

	std::pair<bool, SaveElement> SaveElement::GetNextSibling() const
	{
		SaveElement nextSibling;

		nextSibling.element = element->NextSiblingElement();

		if (nextSibling.element == nullptr)
		{
			return { false, nextSibling };
		}

		return { true, nextSibling };
	}

	std::pair<bool, SaveElement> SaveElement::GetSibling(const char* name) const
	{
		SaveElement sibling;
		
		sibling.element = element->NextSiblingElement(name);

		if (sibling.element == nullptr)
		{
			sibling.element = element->PreviousSiblingElement(name);
		}

		if (sibling.element == nullptr)
		{
			return { false, sibling };
		}

		return { true, sibling };
	}

	const char* SaveElement::GetElementName() const
	{
		return element->Name();
	}

	LibMath::Vector3 SaveElement::GetVector3Element(const char* name) const
	{
		return {element->FirstChildElement(name)->FindAttribute(ATTRIBUTE_VECTOR_X)->FloatValue(),
				element->FirstChildElement(name)->FindAttribute(ATTRIBUTE_VECTOR_Y)->FloatValue(),
				element->FirstChildElement(name)->FindAttribute(ATTRIBUTE_VECTOR_Z)->FloatValue() };
	}

	LibMath::Vector4 SaveElement::GetVector4Element(const char* name) const
	{
		return {element->FirstChildElement(name)->FindAttribute(ATTRIBUTE_VECTOR_X)->FloatValue(),
				element->FirstChildElement(name)->FindAttribute(ATTRIBUTE_VECTOR_Y)->FloatValue(),
				element->FirstChildElement(name)->FindAttribute(ATTRIBUTE_VECTOR_Z)->FloatValue(),
				element->FirstChildElement(name)->FindAttribute(ATTRIBUTE_VECTOR_W)->FloatValue() };
	}

	LibMath::Quaternion SaveElement::GetQuaternionElement(const char* name) const
	{
		return {element->FirstChildElement(name)->FindAttribute(ATTRIBUTE_VECTOR_X)->FloatValue(),
				element->FirstChildElement(name)->FindAttribute(ATTRIBUTE_VECTOR_Y)->FloatValue(),
				element->FirstChildElement(name)->FindAttribute(ATTRIBUTE_VECTOR_Z)->FloatValue(),
				element->FirstChildElement(name)->FindAttribute(ATTRIBUTE_VECTOR_W)->FloatValue() };
	}

	std::tuple<bool, bool, bool> SaveElement::GetVector3BoolElement(const char* name) const
	{
		return {element->FirstChildElement(name)->FindAttribute(ATTRIBUTE_VECTOR_X)->BoolValue(),
						element->FirstChildElement(name)->FindAttribute(ATTRIBUTE_VECTOR_Y)->BoolValue(),
						element->FirstChildElement(name)->FindAttribute(ATTRIBUTE_VECTOR_Z)->BoolValue() };
	}

	const char* SaveElement::GetStringAttribute(const char* name) const
	{
		return element->FindAttribute(name)->Value();
	}

	int SaveElement::GetIntAttribute(const char* name) const
	{
		return element->FindAttribute(name)->IntValue();
	}

	float SaveElement::GetFloatAttribute(const char* name) const
	{
		return element->FindAttribute(name)->FloatValue();
	}

	bool SaveElement::GetBoolAttribute(const char* name) const
	{
		return  element->FindAttribute(name)->BoolValue();
	}


	Serializer::Serializer()
	{
		m_xmlDoc = new tinyxml2::XMLDocument;
	}

	Serializer::~Serializer()
	{
		delete m_xmlDoc;
	}

	void Serializer::ResetSerializer()
	{
		if (m_xmlDoc != nullptr)
		{
			m_xmlDoc->Clear();
		}
	}

	SaveElement Serializer::AddRootElement(const char* elementName)
	{
		SaveElement scene(*m_xmlDoc, elementName);
		
		return scene;
	}

	void Serializer::Serialize(const std::string& filePath, SaveElement& root)
	{
		m_xmlDoc->InsertFirstChild(root.element);

		m_xmlDoc->SaveFile(filePath.c_str());

	}

	bool Serializer::Deserialize(const char* filePath)
	{
		if (m_xmlDoc == nullptr)
		{
			LOG(SOURCE_INFO, EVerbosity::LOG_WARNING, "Scene serializer xmlDoc is unitialized, failed to load : %s", filePath);
			return false;
		}

		
		
		m_xmlDoc->Clear();

		tinyxml2::XMLError error =  m_xmlDoc->LoadFile(filePath);

		if (error != tinyxml2::XML_SUCCESS)
		{
			LOG(SOURCE_INFO, EVerbosity::LOG_ERROR, "Failed to deserialize file %s", filePath);
			return false;
		}

		return true;
	}

	std::pair<bool, Core::SaveElement> Serializer::GetFirstElement(const char* name) const
	{
		SaveElement firstElement;

		firstElement.element = m_xmlDoc->FirstChildElement(name);

		if (firstElement.element == nullptr)
		{
			LOG(SOURCE_INFO, EVerbosity::LOG_WARNING, "Failed to find first element with name %s", name);
			return { false, firstElement };
		}

		return { true, firstElement };
	}

}
