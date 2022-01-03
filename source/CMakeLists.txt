set(LIBRARY_SOURCES 
    tale/tale.hpp tale/tale.cpp tattle/tattle.hpp tattle/tattle.cpp
)

set(ADDITIONAL_EXECTUABLE_SOURCES 
    main.cpp
)

add_executable(${EXECUTABLE_NAME} ${ADDITIONAL_EXECTUABLE_SOURCES} ${LIBRARY_SOURCES})

add_library(${LIBRARY_NAME} STATIC ${LIBRARY_SOURCES})
