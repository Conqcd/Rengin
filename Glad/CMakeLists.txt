
file(GLOB_RECURSE GLAD_SRC *.c *.h)


include_directories(include)
add_library(glad STATIC ${GLAD_SRC})