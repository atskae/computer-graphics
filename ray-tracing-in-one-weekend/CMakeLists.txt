cmake_minimum_required(VERSION 3.15)
# Allow C++17 features
set(CMAKE_CXX_STANDARD 17)

# Project name
project(RayTracer)

# Source files
set(SOURCES "src/main.cpp")

# Compile with debug symbols (for GDB)
#set(CMAKE_BUILD_TYPE Debug)

# Executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include headers
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)
