
cmake_minimum_required(VERSION 3.15)

project(io_expander_controller C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#
# PROJECT_BUILD_MODE controls how this module is built:
#   TEST -> standalone unit test executable (native stubs + Unity + mocks)
#   LIB  -> static library for consumption by a parent application
#
set(PROJECT_BUILD_MODE "TEST" CACHE STRING "Build mode for this module: TEST or LIB")

#
# Module source files - always built, regardless of mode
#
file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

file(GLOB_RECURSE APP_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/inc/*.h"
)

if(PROJECT_BUILD_MODE STREQUAL "TEST")

    #
    # Native stub sources (error.c) - only needed for standalone/test builds.
    # application_defs.h and printf_logging.h are header-only stubs, picked
    # up purely via the include path below.
    #
    file(GLOB_RECURSE NATIVE_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/test/native/src/*.c"
    )

    file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/test/unit/*.c"
    )

    file(GLOB_RECURSE MOCK_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/test/Mocks/*.c"
    )

    add_executable(io_expander_controller
        ${APP_SOURCES}
        ${NATIVE_SOURCES}
        ${TEST_SOURCES}
        ${MOCK_SOURCES}
        test_main.c
        ${PROJECT_SOURCE_DIR}/../../ThirdParty/unity/src/unity.c
        ${PROJECT_SOURCE_DIR}/../../ThirdParty/unity/extras/fixture/src/unity_fixture.c
        ${PROJECT_SOURCE_DIR}/../../ThirdParty/unity/extras/memory/src/unity_memory.c
    )

    target_include_directories(io_expander_controller PRIVATE
        inc
        test/native/inc
        test/Mocks
        ${PROJECT_SOURCE_DIR}/../../ThirdParty/unity/src
        ${PROJECT_SOURCE_DIR}/../../ThirdParty/unity/extras/fixture/src/
        ${PROJECT_SOURCE_DIR}/../../ThirdParty/unity/extras/memory/src
    )

    set_target_properties(io_expander_controller PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
    )

elseif(PROJECT_BUILD_MODE STREQUAL "LIB")

    add_library(io_expander_controller_lib STATIC
        ${APP_SOURCES}
    )

    target_include_directories(io_expander_controller_lib PUBLIC
        inc
    )

    #
    # application_defs.h, printf_logging.h and error.h are NOT provided here.
    # The parent application's build is responsible for exposing them on
    # the include path before this module's sources are compiled.
    #

else()

    message(FATAL_ERROR "Unknown PROJECT_BUILD_MODE '${PROJECT_BUILD_MODE}'. Expected TEST or LIB.")

endif()


