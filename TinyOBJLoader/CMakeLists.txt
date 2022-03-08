
file(GLOB_RECURSE TINIYOBJLOADER_SRC *.cpp *.hpp)


add_library(tinyobjloader STATIC ${TINIYOBJLOADER_SRC})