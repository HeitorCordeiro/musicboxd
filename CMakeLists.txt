cmake_minimum_required(VERSION 3.16)
project(musicboxd VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Warnings
if (MSVC)
  add_compile_options(/W4 /permissive-)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Collect sources under src/
file(GLOB_RECURSE MUSICBOXD_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

add_executable(musicboxd ${MUSICBOXD_SOURCES})

target_include_directories(musicboxd PRIVATE ${CMAKE_SOURCE_DIR}/src)

option(BUILD_TESTS "Build tests" OFF)

message(STATUS "musicboxd project configured. Sources found: ${MUSICBOXD_SOURCES}")
