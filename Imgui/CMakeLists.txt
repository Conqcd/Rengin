include(${PROJECT_SOURCE_DIR}/third_party/glfw.cmake)
file(GLOB IMGUI_SRC1
    *.h
    *.cpp)

file(GLOB_RECURSE IMGUI_SRC2
imgui_impl_opengl3.h
imgui_impl_opengl3_loader.h
imgui_impl_opengl3.cpp
imgui_impl_glfw.h
imgui_impl_glfw.cpp
)

include_directories(../Imgui)
add_library(imgui STATIC ${IMGUI_SRC1} ${IMGUI_SRC2})
target_link_libraries(imgui
    PRIVATE
    glfw)