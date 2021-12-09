
file(GLOB IMGUI_SRC1
    *.h
    *.cpp)

file(GLOB_RECURSE IMGUI_SRC2
imgui_impl_opengl3.h
imgui_impl_opengl3_loader.h
imgui_impl_opengl3.cpp
)

include_directories(../Imgui)
add_library(imgui STATIC ${IMGUI_SRC1} ${IMGUI_SRC2})