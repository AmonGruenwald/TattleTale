set(LIBRARY_SOURCES 
    tale/tale.hpp 
    tale/tale.cpp 
    tale/actor.hpp
    tale/actor.cpp
    tale/kernel.hpp
    tale/kernel.cpp
    tattle/tattle.hpp 
    tattle/tattle.cpp
)

set(ADDITIONAL_EXECTUABLE_SOURCES 
    main.cpp
)

add_library(${LIBRARY_NAME} STATIC ${LIBRARY_SOURCES})


add_executable(${EXECUTABLE_NAME} ${ADDITIONAL_EXECTUABLE_SOURCES})


target_link_libraries(${EXECUTABLE_NAME} ${LIBRARY_NAME})
