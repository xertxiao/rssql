cmake_minimum_required(VERSION 3.12)
project(RSSQL)

# Add include directories
include_directories(include)

# Add source files
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")

# Add executable
add_executable(RSSQL main.cpp ${SOURCE_FILES})
