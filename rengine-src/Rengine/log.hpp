#pragma once
#include "../repch.hpp"
#include "core.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
namespace Rengin
{
class RE_API Log
{
private:
    static std::shared_ptr<spdlog::logger> sp_core_logger;
    static std::shared_ptr<spdlog::logger> sp_client_logger;
public:
    Log(/* args */){}
    ~Log(){}

    static void Init();
    inline static std::shared_ptr<spdlog::logger>& GetClientLogger() {return sp_client_logger;}
    inline static std::shared_ptr<spdlog::logger>& GetCoreLogger(){return sp_core_logger;}
};

}

#define RE_CORE_ERROR(...) ::Rengin::Log::GetCoreLogger()->error(__VA_ARGS__)
#define RE_CORE_INFO(...) ::Rengin::Log::GetCoreLogger()->info(__VA_ARGS__)
#define RE_CORE_WARN(...) ::Rengin::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define RE_CORE_TRACE(...) ::Rengin::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define RE_CORE_FATAL(...) ::Rengin::Log::GetCoreLogger()->fatal(__VA_ARGS__)


#define RE_ERROR(...) ::Rengin::Log::GetClientLogger()->error(__VA_ARGS__)
#define RE_INFO(...) ::Rengin::Log::GetClientLogger()->info(__VA_ARGS__)
#define RE_WARN(...) ::Rengin::Log::GetClientLogger()->warn(__VA_ARGS__)
#define RE_TRACE(...) ::Rengin::Log::GetClientLogger()->trace(__VA_ARGS__)
#define RE_FATAL(...) ::Rengin::Log::GetClientLogger()->fatal(__VA_ARGS__)
