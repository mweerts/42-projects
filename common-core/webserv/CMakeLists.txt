cmake_minimum_required(VERSION 3.20)
project(webserv LANGUAGES CXX)

# C++ Standard to match Makefile
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generate compile_commands.json for LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compiler flags to match Makefile exactly
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -std=c++98")

# Include directories
include_directories(includes)

# Find all .cpp files in srcs directory recursively
file(GLOB_RECURSE SOURCES "srcs/*.cpp")

# Create executable
add_executable(webserv ${SOURCES})

# Custom target to run the program (like 'make run')
add_custom_target(run
    COMMAND ./webserv
    DEPENDS webserv
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
