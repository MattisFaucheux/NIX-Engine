#pragma once

#include <string>
#include <vector>

namespace NIX::Rendering
{
	struct File
	{
		static std::vector<char> ReadFile(const std::string& path);
	};
}
