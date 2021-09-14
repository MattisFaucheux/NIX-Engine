#pragma once

#ifdef GEOMETRICTOOLS_EXPORTS
#	define GEOMETRIC_TOOLS_LIBRARY_API __declspec(dllexport)
#else
#   define GEOMETRIC_TOOLS_LIBRARY_API __declspec(dllimport)
#endif