#pragma once

#ifdef RE_WINDOWS
    #ifdef RE_BUILD_DLL
    #define RE_API __declspec(dllexport)
    #else
    #define RE_API __declspec(dllimport)
    #endif
#else
#error no support for other platforms
#endif