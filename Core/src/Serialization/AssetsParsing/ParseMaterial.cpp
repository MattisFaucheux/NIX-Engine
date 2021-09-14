#include "pch.h"

#include "Serialization/AssetsParsing/ParseMaterial.h"
#include "Serialization/AssetsParsing/JSonParser.h"
#include "Log/Logger.h"

#include <External/rapidjson/writer.h>
#include "External/rapidjson/document.h"

#include <fstream>

namespace NIX::Core
{
    ParseMaterial::ParseMaterial(const std::string& filePath)
    {
        rapidjson::Document document = JSonParser::ReadFile(filePath);

        if (!document.IsObject())
        {
            return;
        }
    	
        if(!document.HasMember(MEMBER_ALBEDO_MAP))
        {
            LOG(SOURCE_INFO, Core::EVerbosity::LOG_WARNING, "%s does not have a %s member", filePath, MEMBER_ALBEDO_MAP);
            return;
        }
        if (!document[MEMBER_ALBEDO_MAP].IsString())
        {
            LOG(SOURCE_INFO, Core::EVerbosity::LOG_WARNING, "Member %s is not a string", MEMBER_ALBEDO_MAP);
            return;
        }
    	
        if (!document.HasMember(MEMBER_NORMAL_MAP))
        {
            LOG(SOURCE_INFO, Core::EVerbosity::LOG_WARNING, "%s does not have a %s member", filePath, MEMBER_NORMAL_MAP);
            return;
        }
        if (!document[MEMBER_NORMAL_MAP].IsString())
        {
            LOG(SOURCE_INFO, Core::EVerbosity::LOG_WARNING, "Member %s is not a string", MEMBER_NORMAL_MAP);
            return;
        }
    	
        albedoMap = document[MEMBER_ALBEDO_MAP].GetString();
        normalMap = document[MEMBER_NORMAL_MAP].GetString();
    }
    void ParseMaterial::CreateMaterial(const char* assetPath, const char* albedo, const  char* normal)
    {    	
        rapidjson::StringBuffer outputString;
        rapidjson::Writer<rapidjson::StringBuffer> writer(outputString);

        writer.StartObject();
            	
    	// Albedo
        writer.Key(MEMBER_ALBEDO_MAP);

    	if (albedo == nullptr)
    	{
	        writer.String("");
        }
        else
        {
	        writer.String(albedo);
        }

    	// Normal
        writer.Key(MEMBER_NORMAL_MAP);
        if (normal == nullptr)
        {
            writer.String("");
        }
        else
        {
            writer.String(normal);
        }
    	
        writer.EndObject();
    	
        if (writer.IsComplete())
        {        	
            std::ofstream outfile(assetPath);
            outfile << outputString.GetString();
            outfile.close();

            return;
        }

        LOG(SOURCE_INFO, EVerbosity::LOG_ERROR, "Failed to export material in json. Asset path : %s, albedo path :%s, normal path : %s", assetPath, albedo, normal);
    }
}
