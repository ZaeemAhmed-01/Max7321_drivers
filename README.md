# IO Expander Controller

A driver module for the MAX7321 I2C IO expander, providing initialization,
GPIO read/write, and interrupt-driven input state change callbacks.

## How to Include It in Your Project

This module needs the following files to be provided by the parent project:

- `error.h` / `error.c`
- `application_defs.h`
- `printf_logging.h`

Add the following to your project's `CMakeLists.txt`:

```cmake
set(PROJECT_BUILD_MODE "LIB")

# IO expander controller module
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Modules/io_expander_controller)

# Provide the error, printf logging, and application defs headers
target_include_directories(io_expander_controller_lib PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Appli/App-Logic/main/inc  # {TEMPLATE}
)

# Provide the error source file to be compiled into your build
target_sources(io_expander_controller_lib PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Appli/App-Logic/main/src/error.c  # {TEMPLATE}
)
```

## Interrupt Callbacks

- Callbacks for IOs that are not registered will simply be ignored.
- **Note:** A callback function is only called if that pin is configured as an input.
- **Note:** A callback function is only called when the state of the pin changes, not on any rising or falling edge specifically.
