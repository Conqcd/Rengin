#include "log.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Rengin
{

    
std::shared_ptr<spdlog::logger> Log::sp_core_logger;
std::shared_ptr<spdlog::logger> Log::sp_client_logger;

void Log::Init()
{
    spdlog::set_pattern("%^[%T] %n: %v%$");
    sp_core_logger = std::make_shared<spdlog::logger>(*spdlog::stdout_color_mt("Rengin"));
    // sp_core_logger = (spdlog::stdout_color_mt("Rengin"));
    sp_core_logger->set_level(spdlog::level::trace);

    sp_client_logger = std::make_shared<spdlog::logger>(*spdlog::stdout_color_mt("Clinet"));
    // sp_client_logger = (spdlog::stdout_color_mt("Clinet"));
    sp_client_logger->set_level(spdlog::level::trace);
}

}