set(BINARY ${CMAKE_PROJECT_NAME})

set(SOURCES 
    Example.hpp Example.cpp
)

add_executable(${BINARY}_run main.cpp ${SOURCES})

add_library(${BINARY}_lib STATIC ${SOURCES})
