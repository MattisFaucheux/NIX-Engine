#include "pch.h"

#include "Serialization/AssetsParsing/JSonParser.h"

#include <External/rapidjson/document.h>

#include <filesystem>
#include <fstream>

#include "Log/Logger.h"

namespace NIX::Core
{
    rapidjson::Document JSonParser::ReadFile(const std::string& path)
	{
        std::ifstream file(path);

        if (!file.is_open())
        {
            LOG(SOURCE_INFO, EVerbosity::LOG_WARNING, "Failed to open file : %s when parsing JSon", path.c_str());
            return nullptr;
        }

        const std::string jsonData((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        rapidjson::Document document;
        document.Parse(jsonData.c_str());
        return document;
	}

    void JSonParser::TakeOutExtension(std::string& path)
    {
        path.resize(path.find_first_of('.'));
    }
}
