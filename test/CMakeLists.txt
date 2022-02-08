include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

set(SOURCES tale-test.cpp tattle-test.cpp)

add_executable(${TEST_EXECUTABLE_NAME} ${SOURCES})

add_custom_command(
        TARGET ${TEST_EXECUTABLE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/../source/tale/resources
                $<TARGET_FILE_DIR:${TEST_EXECUTABLE_NAME}>/tale/resources)

target_link_libraries(${TEST_EXECUTABLE_NAME} ${LIBRARY_NAME} gtest_main)


include(GoogleTest)
gtest_discover_tests(${TEST_EXECUTABLE_NAME})