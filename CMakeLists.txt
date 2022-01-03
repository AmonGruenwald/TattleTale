cmake_minimum_required(VERSION 3.14)

project(TattleTale VERSION 0.0)

set(CMAKE_CXX_STANDARD 17)


set(EXECUTABLE_NAME ${CMAKE_PROJECT_NAME})
set(LIBRARY_NAME ${CMAKE_PROJECT_NAME}_library)
set(TEST_EXECUTABLE_NAME ${CMAKE_PROJECT_NAME}_test)


configure_file(Doxyfile.in Doxyfile)

include_directories(source)

add_subdirectory(source)
add_subdirectory(test)