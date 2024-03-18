cmake_minimum_required(VERSION 3.10.0)
project(raytracer CXX)
set(CMAKE_CXX_COMPILER /bin/g++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Set the include directory
include_directories(include)

# Add executable
add_executable(raytracer ${SOURCES})