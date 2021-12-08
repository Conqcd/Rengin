#pragma once

#ifdef RE_WINDOWS
    #ifdef RE_BUILD_DLL
    #define RE_API __declspec(dllexport)
    #else
    #define RE_API __declspec(dllimport)
    #endif
#else
#define RE_API
#error no support for other platforms
#endif

#define BIT(x) (1<<x)