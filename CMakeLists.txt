cmake_minimum_required(VERSION 3.10)
project(MyCLib C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -D_DEFAULT_SOURCE -Wno-missing-braces")

find_package(raylib QUIET)
if (NOT raylib_FOUND)
    find_package(PkgConfig)
    if (PkgConfig_FOUND)
        pkg_check_modules(raylib raylib)
    endif()
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)
if (raylib_INCLUDE_DIRS)
    include_directories(${raylib_INCLUDE_DIRS})
endif()

set(SOURCES
    src/main.c
    src/ui.c
    src/string_utils.c
    src/math_utils.c
)

add_executable(main ${SOURCES})

if (raylib_LIBRARIES)
    target_link_libraries(main ${raylib_LIBRARIES})
elseif (raylib_FOUND)
    target_link_libraries(main raylib)
else()
    if (APPLE)
        target_link_libraries(main
            raylib
            -framework CoreVideo
            -framework IOKit
            -framework Cocoa
            -framework GLUT
            -framework OpenGL
            -lm
            -lpthread
        )
    elseif (WIN32)
        target_link_libraries(main raylib opengl32 gdi32 winmm m)
    else()
        target_link_libraries(main raylib m pthread dl)
    endif()
endif()
# Copy resources to build directory
file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})
