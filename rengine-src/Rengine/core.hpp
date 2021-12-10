#pragma once
#include "../repch.hpp"
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

#ifdef RE_DEBUG
    #define RE_ENABLE_ASSERT
#endif 

#ifdef RE_ENABLE_ASSERT
    #define RE_CORE_ASSERT(x,...) { if(!x){ RE_CORE_ERROR("Assert fail {0}",__VA_ARGS__); __debugbreak(); } }
    #define RE_ASSERT(x,...) { if(!x){ RE_ERROR("Assert fail {0}",__VA_ARGS__); __debugbreak(); } }
#else
    #define RE_CORE_ASSERT(x,...)
    #define RE_ASSERT(x,...)
#endif


#define LEFT(x) (1<<x)

#define RE_BIND_FUNC_EVENT_1(fn) std::bind(&fn , this , std::placeholders::_1)