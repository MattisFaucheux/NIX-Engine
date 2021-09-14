#pragma once

#ifdef RENDERING_EXPORTS
#	define RENDERING_LIBRARY_API __declspec(dllexport)
#else
#   define RENDERING_LIBRARY_API __declspec(dllimport)
#endif