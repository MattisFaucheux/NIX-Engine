#include "pch.h"

#include "Tools/File.h"

#include <fstream>

namespace NIX::Rendering
{
	std::vector<char> File::ReadFile(const std::string& path)
	{
        std::ifstream file(path, std::ios::ate | std::ios::binary);

        if (!file.is_open())
        {
            std::vector<char> buffer(0);
            return buffer;
        }

        const size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();
        return buffer;
	}
}
