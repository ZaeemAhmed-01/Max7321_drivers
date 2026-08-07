cmake_minimum_required(VERSION 3.15)

project(io_expander_controller C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 99)

#
# Collect source files
#
file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

file(GLOB_RECURSE APP_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/inc/*.h"
)

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/test/unit/*.c"
)

file(GLOB_RECURSE MOCK_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/test/Mocks/*.c"
)

#
# test application 
#
add_executable(io_expander_controller
    ${APP_SOURCES}
    ${TEST_SOURCES}
    ${MOCK_SOURCES}
    test_main.c
    ${PROJECT_SOURCE_DIR}/../../ThirdParty/unity/src/unity.c
    ${PROJECT_SOURCE_DIR}/../../ThirdParty/unity/extras/fixture/src/unity_fixture.c
    ${PROJECT_SOURCE_DIR}/../../ThirdParty/unity/extras/memory/src/unity_memory.c
)


target_include_directories(io_expander_controller PRIVATE
    inc
    test/Mocks
    ${PROJECT_SOURCE_DIR}/../../ThirdParty/unity/src
    ${PROJECT_SOURCE_DIR}/../../ThirdParty/unity/extras/fixture/src/
    ${PROJECT_SOURCE_DIR}/../../ThirdParty/unity/extras/memory/src
)


#
# Place executables in bin/
#
set_target_properties(io_expander_controller PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
)




# create the library to be included in the application.