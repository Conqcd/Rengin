if (WIN32)
add_definitions(-DRE_WINDOWS)
endif()

add_definitions(-DRE_ENABLE_ASSERT)
# if(WIN32)
# add_custom_command(TARGET ${proj_name} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#         "${DuiLib_dll_path}/bin/$<CONFIGURATION>/DuiLib.dll"
#         "${bin_dir}/$<CONFIGURATION>")
# endif()

include_directories(../rengine-src)
include_directories(../Imgui)
include_directories(../Stb)
include_directories(../Entt)
add_executable(Editor
    main.cpp
    EditorLayer.cpp
    Panels/SceneHierarchyPanel.cpp
    )

target_link_libraries(Editor
    PRIVATE 
    rengine
    spdlog
    opengl32
    glfw
    glm
    imgui
    yaml-cpp
    )