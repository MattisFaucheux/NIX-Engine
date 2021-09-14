#pragma once

#include "API/DLLExportCore.h"

#include <string>

#define MEMBER_ALBEDO_MAP "albedoMap"
#define MEMBER_NORMAL_MAP "normalMap"

namespace NIX::Core
{
	struct ParseMaterial
	{
		std::string albedoMap = "";
		std::string normalMap = "";

		CORE_LIBRARY_API ParseMaterial(const std::string& filePath);

		CORE_LIBRARY_API static void CreateMaterial(const  char* assetPath, const char* albedo, const char* normal);
	};
}
