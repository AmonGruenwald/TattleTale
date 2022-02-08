set(LIBRARY_SOURCES 
    tale/tale.hpp 
    tale/tale.cpp 
    tale/actor.hpp
    tale/actor.cpp
    tale/kernels/kernel.hpp
    tale/kernels/kernel.cpp
    tale/kernels/action.hpp
    tale/kernels/action.cpp
    tale/kernels/emotion.hpp
    tale/kernels/emotion.cpp
    tale/kernels/goal.hpp
    tale/kernels/goal.cpp
    tale/kernels/interaction.hpp
    tale/kernels/interaction.cpp
    tale/kernels/relationship.hpp
    tale/kernels/relationship.cpp
    tale/kernels/resource.hpp
    tale/kernels/resource.cpp
    tale/kernels/trait.hpp
    tale/kernels/trait.cpp
    tattle/tattle.hpp 
    tattle/tattle.cpp
)


include(FetchContent)

FetchContent_Declare(json
  URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz
)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()


set(ADDITIONAL_EXECTUABLE_SOURCES 
    main.cpp
)

add_library(${LIBRARY_NAME} STATIC ${LIBRARY_SOURCES})


add_executable(${EXECUTABLE_NAME} ${ADDITIONAL_EXECTUABLE_SOURCES})


target_link_libraries(${EXECUTABLE_NAME} ${LIBRARY_NAME} nlohmann_json::nlohmann_json)
