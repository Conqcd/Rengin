include(${PROJECT_SOURCE_DIR}/third_party/fmt.cmake)
include(${PROJECT_SOURCE_DIR}/third_party/spdlog.cmake)
include(${PROJECT_SOURCE_DIR}/third_party/glfw.cmake)

if (WIN32)
add_definitions(-DRE_WINDOWS)
endif()
add_definitions(-DRE_ENABLE_ASSERT)
add_definitions(-DRE_BUILD_DLL)


file(GLOB_RECURSE RENGINE_SRC *.cpp *.hpp)
include_directories(../Glad/include)
add_library(rengine SHARED ${RENGINE_SRC})
target_link_libraries(rengine
    PRIVATE
    spdlog
    opengl32
    glfw
    glad)