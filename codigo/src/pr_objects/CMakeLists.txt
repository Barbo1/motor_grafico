add_library (objsrc_probjects OBJECT)
target_include_directories (objsrc_probjects PUBLIC ${CMAKE_SOURCE_DIR}/headers)
target_sources (objsrc_probjects PRIVATE circle.cpp square.cpp)
