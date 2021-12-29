if (WIN32)
add_definitions(-DRE_WINDOWS)
endif()

add_definitions(-DRE_ENABLE_ASSERT)

include_directories(../rengine-src)
include_directories(../Imgui)
include_directories(../Stb)
include_directories(../Entt)
add_executable(Editor main.cpp EditorLayer.cpp)
target_link_libraries(Editor
    PRIVATE 
    rengine
    spdlog
    opengl32
    glfw
    glm
    imgui
    )