# https://stackoverflow.com/questions/8304190/cmake-with-include-and-source-paths-basic-setup

cmake_minimum_required(VERSION 3.1)
cmake_policy(SET CMP0048 NEW)

project(clock
    VERSION 0.1
    LANGUAGES C CXX
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/dist)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/dist)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/dist)
set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/dist)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

file(GLOB srcFiles
    "${PROJECT_SOURCE_DIR}/include/*.h"
    "${PROJECT_SOURCE_DIR}/src/*.c"
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable(${PROJECT_NAME} ${srcFiles})
# target_link_libraries(${PROJECT_NAME} )