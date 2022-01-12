

cmake_minimum_required(VERSION 3.17)
project(Projekt C)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES wielomiany.c wielomiany.h main.c kalkulator.c kalkulator.h)
add_executable(Projekt ${SOURCE_FILES})

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER})

target_link_libraries(Projekt ${GTK3_LIBRARIES})