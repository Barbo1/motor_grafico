cmake_minimum_required (VERSION 3.10)
project (MotorGrafico)
set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_COMPILER g++-14)
set (CMAKE_OBJCXX_STANDARD_REQUIRED True)
set (CMAKE_CXX_FLAGS "-O3 -Wall -Werror -msse -msse2 -msse3 -msse4.1 -mavx -mfma -mavx2")
find_package (SDL2 REQUIRED)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/compiled)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/compiled)

add_subdirectory (src)
add_subdirectory (prueba)
