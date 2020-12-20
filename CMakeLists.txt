cmake_minimum_required(VERSION 3.6)
project(oct-tree)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cc)
add_executable(oct-tree ${SOURCE_FILES})

set(YOU_NEED_X11 1)
set(YOU_NEED_PNG 1)

if (${YOU_NEED_PNG} EQUAL 1)
    message(STATUS "Looking for libpng...")
    find_package(PNG REQUIRED)
    include_directories(${PNG_INCLUDE_DIR})
    target_link_libraries (oct-tree ${PNG_LIBRARY})
    target_compile_definitions(oct-tree PRIVATE cimg_use_png=1)
endif()

if (${YOU_NEED_X11} EQUAL 1)
    message(STATUS "Looking for X11...")
    find_package(X11 REQUIRED)
    include_directories(${X11_INCLUDE_DIR})
    target_link_libraries(oct-tree ${X11_LIBRARIES})
else()
    target_compile_definitions(oct-tree PRIVATE cimg_display=0)
endif()