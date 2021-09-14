#pragma once
#include <string>
#include "API/DLLExportCore.h"
#include "External/tinyxml/tinyxml2.h"

#define ATTRIBUTE_VECTOR_X "x"
#define ATTRIBUTE_VECTOR_Y "y"
#define ATTRIBUTE_VECTOR_Z "z"
#define ATTRIBUTE_VECTOR_W "w"

namespace LibMath {
	struct Vector4;
	struct Quaternion;
	struct Vector3;
}

namespace tinyxml2 {
	class XMLElement;
}

namespace NIX::Core
{
	struct SaveElement
	{
		tinyxml2::XMLElement* element;

		SaveElement() = default;
		SaveElement(tinyxml2::XMLDocument& xmlDoc,const char* elementName);
		CORE_LIBRARY_API SaveElement(SaveElement& parent, const std::string& name);

		CORE_LIBRARY_API void AddIntAttribute(const char* name, const int value);
		CORE_LIBRARY_API void AddFloatAttribute(const char* name, const float value);
		CORE_LIBRARY_API void AddStringAttribute(const char* name, const char* value);
		CORE_LIBRARY_API void AddBoolAttribute(const char* name, const bool value);
		
		CORE_LIBRARY_API void AddVec3Element(const std::string& name, const LibMath::Vector3& value);
		CORE_LIBRARY_API void AddVec4Element(const std::string& name, const LibMath::Quaternion& value);
		CORE_LIBRARY_API void AddVec4Element(const std::string& name, const LibMath::Vector4& value);

		CORE_LIBRARY_API void AddVec3BoolElement(const std::string& name, const std::tuple<bool, bool, bool>& value);
		CORE_LIBRARY_API void AddVec3BoolElement(const std::string& name, bool x, bool y, bool z);

		CORE_LIBRARY_API [[nodiscard]] std::pair<bool, SaveElement> GetFirstChild() const;
		CORE_LIBRARY_API [[nodiscard]] std::pair<bool, SaveElement> GetChild(const char* name) const ;
		CORE_LIBRARY_API [[nodiscard]] std::pair<bool, SaveElement> GetNextSibling() const;
		CORE_LIBRARY_API [[nodiscard]] std::pair<bool, SaveElement> GetSibling(const char* name) const;

		CORE_LIBRARY_API [[nodiscard]] const char* GetElementName() const;

		CORE_LIBRARY_API [[nodiscard]] LibMath::Vector3 GetVector3Element(const char* name) const;
		CORE_LIBRARY_API [[nodiscard]] LibMath::Vector4 GetVector4Element(const char* name) const;
		CORE_LIBRARY_API [[nodiscard]] LibMath::Quaternion GetQuaternionElement(const char* name) const;
		CORE_LIBRARY_API [[nodiscard]] std::tuple<bool, bool, bool> GetVector3BoolElement(const char* name) const;
		
		CORE_LIBRARY_API [[nodiscard]] const char* GetStringAttribute(const char* name) const;
		CORE_LIBRARY_API [[nodiscard]] int GetIntAttribute(const char* name) const;
		CORE_LIBRARY_API [[nodiscard]] float GetFloatAttribute(const char* name) const;
		CORE_LIBRARY_API [[nodiscard]] bool GetBoolAttribute(const char* name) const;
		
	};
	
	class Serializer
	{
	private:
		tinyxml2::XMLDocument* m_xmlDoc;
	public:
		CORE_LIBRARY_API Serializer();
		CORE_LIBRARY_API ~Serializer();

		CORE_LIBRARY_API void ResetSerializer();

		CORE_LIBRARY_API SaveElement AddRootElement(const char* elementName);
		
		CORE_LIBRARY_API void Serialize(const std::string& filePath, SaveElement& root);
		CORE_LIBRARY_API bool Deserialize(const char* filePath);

		CORE_LIBRARY_API [[nodiscard]] std::pair<bool, Core::SaveElement> GetFirstElement(const char* name) const;
	};
}
