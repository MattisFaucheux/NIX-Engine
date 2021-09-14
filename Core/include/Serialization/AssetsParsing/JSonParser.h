#pragma once
	
#include <string>
#include <External/rapidjson/document.h>

#include "API/DLLExportCore.h"

#define MATERIAL_FOLDER "Assets\\Materials\\"

namespace NIX::Core
{
	struct JSonParser
	{		
		static rapidjson::Document ReadFile(const std::string& path);

		CORE_LIBRARY_API static void TakeOutExtension(std::string& path);
	};
}
