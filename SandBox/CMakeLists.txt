# include(${PROJECT_SOURCE_DIR}/third_party/fmt.cmake)
# include(${PROJECT_SOURCE_DIR}/third_party/spdlog.cmake)
# include(${PROJECT_SOURCE_DIR}/third_party/glfw.cmake)
# include(${PROJECT_SOURCE_DIR}/third_party/glm.cmake)
if (WIN32)
add_definitions(-DRE_WINDOWS)
endif()

add_definitions(-DRE_ENABLE_ASSERT)

include_directories(../rengine-src)
include_directories(../Imgui)
add_executable(Rengin main.cpp)
target_link_libraries(Rengin
    PRIVATE 
    rengine
    spdlog
    opengl32
    glfw
    glm
    imgui
    )