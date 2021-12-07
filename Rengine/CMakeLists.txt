include(${PROJECT_SOURCE_DIR}/third_party/spdlog.cmake)

if (WIN32)
add_definitions(-DRE_WINDOWS)
endif()

add_definitions(-DRE_BUILD_DLL)


file(GLOB RENGINE_SRC *.cpp *.hpp)

add_library(rengine SHARED ${RENGINE_SRC})