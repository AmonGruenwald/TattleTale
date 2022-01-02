set(BINARY ${CMAKE_PROJECT_NAME})

set(SOURCES 
    tale/tale.hpp tale/tale.cpp tattle/tattle.hpp tattle/tattle.cpp
)

add_executable(${BINARY}_run main.cpp ${SOURCES})

add_library(${BINARY}_lib STATIC ${SOURCES})
