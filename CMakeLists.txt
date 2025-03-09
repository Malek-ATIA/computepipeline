cmake_minimum_required(VERSION 3.16)
project(ComputePipeline LANGUAGES CXX)

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable strict warnings (works for macOS & Windows with MSVC)
if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")  # Strict warnings for MSVC
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")  # GCC/Clang
endif()

# macOS-specific settings
if(APPLE)
    message(STATUS "Compiling on macOS")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

# Windows-specific settings (for MSVC)
if(WIN32)
    message(STATUS "Compiling on Windows")
    add_definitions(-D_USE_MATH_DEFINES)  # Ensure math constants work
endif()

# Define source files
set(SOURCES
    src/main.cpp
    src/ComputePipeline.hpp
    src/Actions.hpp
)

# Add the executable
add_executable(ComputePipeline ${SOURCES})

# Enable optimizations for Release builds
if(CMAKE_BUILD_TYPE MATCHES Release)
    message(STATUS "Using Release build optimizations")
    if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2")  # Optimize for MSVC
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")  # Optimize for GCC/Clang
    endif()
endif()
