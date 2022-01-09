include(${PROJECT_SOURCE_DIR}/third_party/fmt.cmake)
include(${PROJECT_SOURCE_DIR}/third_party/spdlog.cmake)
include(${PROJECT_SOURCE_DIR}/third_party/glfw.cmake)
include(${PROJECT_SOURCE_DIR}/third_party/glm.cmake)
include(${PROJECT_SOURCE_DIR}/third_party/yaml_cpp.cmake)

if (WIN32)
add_definitions(-DRE_WINDOWS)
endif()
# add_definitions(-DRE_ENABLE_ASSERT)
add_definitions(-DRE_BUILD_DLL)


file(GLOB_RECURSE RENGINE_SRC *.cpp *.hpp)

# message(${RENGINE_SRC})

include_directories(../Glad/include)
include_directories(../Imgui)
include_directories(../ImGuizmo)
include_directories(../rengine-src)
include_directories(../stb)
include_directories(../Entt)
# add_library(rengine SHARED ${RENGINE_SRC})
add_library(rengine STATIC ${RENGINE_SRC})
target_link_libraries(rengine
    PRIVATE
    spdlog
    opengl32
    glfw
    glad
    imgui
    imguizmo
    glm
    yaml-cpp
    )