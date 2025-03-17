cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

# Set the project name
project(graphical_template)

# Override raylib's cmake minimum version requirement
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0135 NEW)

# We don't want raylib's examples built
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Try to find a locally installed raylib
find_package(raylib 5.0 QUIET)

# If not found, download and build raylib
if(NOT raylib_FOUND)
    # Include FetchContent module
    include(FetchContent)
    
    # Download raylib 5.0
    FetchContent_Declare(
        raylib
        URL https://github.com/raysan5/raylib/archive/refs/tags/5.0.tar.gz
        OVERRIDE_FIND_PACKAGE
    )
    
    # Make raylib available
    FetchContent_GetProperties(raylib)
    if(NOT raylib_POPULATED)
        set(FETCHCONTENT_QUIET OFF)
        FetchContent_Populate(raylib)
        
        # These settings help avoid the CMake version errors
        set(OLD_CMAKE_POLICY_DEFAULT_CMP0012 ${CMAKE_POLICY_DEFAULT_CMP0012})
        set(CMAKE_POLICY_DEFAULT_CMP0012 NEW)
        
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
        
        set(CMAKE_POLICY_DEFAULT_CMP0012 ${OLD_CMAKE_POLICY_DEFAULT_CMP0012})
    endif()
endif()

# Create the game executable
add_executable(main 
    ./src/main.cpp 
    ./src/game.h 
    ./src/game.cpp
)

# Link raylib to the main executable
target_link_libraries(main PRIVATE raylib)

# Make main find the raylib headers
target_include_directories(main PRIVATE "${raylib_SOURCE_DIR}/src")