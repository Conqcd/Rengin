#pragma once
#include <memory>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    #if defined(WIN64) || defined(_WIN64) || defined(__WIN64__)
        /* Windows x64 */
        #define RE_PLATFORM_WINDOWS
    #else
        /* Windows x86 */
        #error "no support for other x86 Windows"
    #endif
#elif defined(__APPLE__) || defined(__MACH__)
    #include <TargetConditionals.h>

    #if TARGET_IPHONE_SIMULATOR == 1
        #error "IOS Simulator is not supported!"
    #elif TARGET_OS_IPHONE == 1
        #define RE_PLATFORM_IOS
        #error "IOS is not supported!"
    #elif TARGET_OS_MAC == 1
        #define RE_PLATFORM_MACOS
        #error "MacOS is not supported!"
    #else
        #error "Unkowned Apple Platform!"
    #endif
#elif defined(__ANDROID__)
    #define RE_PLATFORM_ANDROID
    #error "Android is not supported!"
#else
    #error "Unkowned Platform!"
#endif


#ifdef RE_PLATFORM_WINDOWS
    #ifdef RE_DYNAMICS_LINKS
        #ifdef RE_BUILD_DLL
        #define RE_API __declspec(dllexport)
        #else
        #define RE_API __declspec(dllimport)
        #endif
    #else
        #define RE_API
    #endif
#else
    #define RE_API
    #error "no support for other platforms"
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

namespace Rengin
{
    template<typename T>
    using Scope = std::unique_ptr<T>;
    
    template<typename T,typename ... Args>
    constexpr Scope<T> CreateScope(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;
    
    template<typename T,typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
} // namespace Rengin
