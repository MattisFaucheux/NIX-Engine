#pragma once

#ifdef SOUND_EXPORTS
#	define SOUND_LIBRARY_API __declspec(dllexport)
#else
#   define SOUND_LIBRARY_API __declspec(dllimport)
#endif