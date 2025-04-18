cmake_minimum_required(VERSION 3.25)

project(accessing_cmake
    LANGUAGES C CXX
)

message(STATUS "[====]the C compiler: ${CMAKE_C_COMPILER}")
message(STATUS "[====]the C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "[====]the CUDA compiler: ${CMAKE_CUDA_COMPILER}")

# VSCode's settings.json
# "cmake.debugConfig": {
# "args": [
# "<argument1>",
# "<argument2>"
# ]
# }

# Option to exclude driver application from build.
set(_BUILD_DRIVER_APPLICATION
    TRUE
    CACHE BOOL "Whether to include driver application in build. Default: True"
)

# Hide this option from GUI's by default.
mark_as_advanced(_BUILD_DRIVER_APPLICATION)

# MSVC requires these two flags to be set in order to link driver executable
# against shared library.
# The topic is about symbol visibility will be further discussed in Chapter 3.
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message(VERBOSE "Windows environment detected, overriding default symbol visibility to `visible` instead of `hidden`")
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif()

add_subdirectory(component_interface)
add_subdirectory(component1)
add_subdirectory(component2)

# Include driver_app subdirectory if CHAPTER2_BUILD_DRIVER_APPLICATION flag is
# enabled.
if(_BUILD_DRIVER_APPLICATION)
    add_subdirectory(driver_app)
endif()
