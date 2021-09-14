#pragma once


#ifdef CORE_EXPORTS
#	define CORE_LIBRARY_API __declspec(dllexport)
#else
#   define CORE_LIBRARY_API __declspec(dllimport)
#endif
