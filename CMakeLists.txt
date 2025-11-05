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

# Prefer system-installed nlohmann_json (header-only). This is optional but recommended for robust JSON parsing.
find_package(nlohmann_json QUIET)
if (nlohmann_json_FOUND)
  target_link_libraries(musicboxd PRIVATE nlohmann_json::nlohmann_json)
else()
  message(WARNING "nlohmann_json not found. JSON parsing will fail unless installed. On Debian/Ubuntu: sudo apt install nlohmann-json3-dev")
endif()

option(BUILD_TESTS "Build tests" OFF)

message(STATUS "musicboxd project configured. Sources found: ${MUSICBOXD_SOURCES}")
