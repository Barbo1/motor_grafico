cmake_minimum_required (VERSION 3.10)
project (MotorGrafico)
set (CMAKE_CXX_STANDARD 23)
set (CMAKE_OBJCXX_STANDARD_REQUIRED True)
find_package (SDL2 REQUIRED)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/compiled)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/compiled)

add_subdirectory (src)
add_subdirectory (prueba)
