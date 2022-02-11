set(LIBRARY_SOURCES 
    tale/tale.hpp 
    tale/tale.cpp 
    tale/actor.hpp
    tale/actor.cpp
    tale/school.hpp
    tale/school.cpp
    tale/course.hpp
    tale/course.cpp
    tale/setting.hpp
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
    tale/globals/interactionstore.hpp
    tale/globals/interactionstore.cpp
    tale/globals/random.cpp
    tale/globals/random.hpp
    tattle/tattle.hpp 
    tattle/tattle.cpp
)

set(ADDITIONAL_EXECTUABLE_SOURCES 
    main.cpp
)

include(FetchContent)

FetchContent_Declare(json
  URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz
)
FetchContent_MakeAvailable(json)

FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG master
)
FetchContent_MakeAvailable(fmt)



add_library(${LIBRARY_NAME} STATIC ${LIBRARY_SOURCES})


target_link_libraries(${LIBRARY_NAME} nlohmann_json::nlohmann_json fmt::fmt )

add_executable(${EXECUTABLE_NAME} ${ADDITIONAL_EXECTUABLE_SOURCES})

add_custom_command(
        TARGET ${EXECUTABLE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/tale/resources
                $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/tale/resources)

target_link_libraries(${EXECUTABLE_NAME} ${LIBRARY_NAME})
