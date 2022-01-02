cmake_minimum_required(VERSION 3.14)

project(TattleTale VERSION 0.0)

set(CMAKE_CXX_STANDARD 17)

configure_file(Doxyfile.in Doxyfile)

include_directories(source)

add_subdirectory(source)
add_subdirectory(test)