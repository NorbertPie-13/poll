cmake_minimum_required(VERSION 3.15)

project(poll LANGUAGES C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_subdirectory(server)
add_subdirectory(client)