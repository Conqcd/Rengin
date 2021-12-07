if (WIN32)
add_definitions(-DRE_WINDOWS)
endif()

include_directories(../Rengine)
add_executable(Rengin main.cpp)
target_link_libraries(Rengin
    PRIVATE 
    rengine
    spdlog)