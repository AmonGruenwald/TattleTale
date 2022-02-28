set(LIBRARY_SOURCES 
    tale/talecore.hpp
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
    tale/kernels/resourcekernels/resource.hpp
    tale/kernels/resourcekernels/resource.cpp
    tale/kernels/resourcekernels/emotion.hpp
    tale/kernels/resourcekernels/emotion.cpp
    tale/kernels/resourcekernels/relationship.hpp
    tale/kernels/resourcekernels/relationship.cpp
    tale/kernels/goal.hpp
    tale/kernels/goal.cpp
    tale/kernels/interactions/interaction.hpp
    tale/kernels/interactions/interaction.cpp
    tale/kernels/interactions/interactionprototype.hpp
    tale/kernels/interactions/interactionrequirement.hpp
    tale/kernels/interactions/interactioncontexttype.hpp
    tale/kernels/interactions/interactiontendency.hpp
    tale/kernels/trait.hpp
    tale/kernels/trait.cpp
    tale/globals/interactionstore.hpp
    tale/globals/interactionstore.cpp
    tale/globals/random.hpp
    tale/globals/random.cpp
    tale/globals/chronicle.hpp
    tale/globals/chronicle.cpp
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

FetchContent_Declare(rang
  GIT_REPOSITORY https://github.com/agauniyal/rang.git
  GIT_TAG master
)
FetchContent_MakeAvailable(rang)



add_library(${LIBRARY_NAME} STATIC ${LIBRARY_SOURCES})


target_link_libraries(${LIBRARY_NAME} nlohmann_json::nlohmann_json fmt::fmt rang)

add_executable(${EXECUTABLE_NAME} ${ADDITIONAL_EXECTUABLE_SOURCES})
# Uncomment/comment out for prints
#target_compile_definitions(${LIBRARY_NAME} PUBLIC TALE_DEBUG_PRINT_OUTPUT)
target_compile_definitions(${LIBRARY_NAME} PUBLIC TALE_ERROR_PRINT_OUTPUT)
#target_compile_definitions(${LIBRARY_NAME} PUBLIC TALE_VERBOSE_PRINT_OUTPUT)

target_compile_definitions(${LIBRARY_NAME} PUBLIC NOMINMAX)


add_custom_command(
        TARGET ${EXECUTABLE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/tale/resources
                $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/tale/resources)

target_link_libraries(${EXECUTABLE_NAME} ${LIBRARY_NAME})
