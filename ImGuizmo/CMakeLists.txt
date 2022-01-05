file(GLOB IMGUIZMO
    *.h
    *.cpp)

include_directories(../Imgui)
add_library(imguizmo STATIC ${IMGUIZMO})
target_link_libraries(imguizmo
    PRIVATE
    imgui
    )