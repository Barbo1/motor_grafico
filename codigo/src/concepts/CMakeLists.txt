file(GLOB_RECURSE THEFILES "texture/*.cpp")

add_library (objsrc_concepts OBJECT)
target_include_directories (objsrc_concepts PUBLIC ${CMAKE_SOURCE_DIR}/headers)
target_sources (objsrc_concepts PRIVATE primitives.cpp physical.cpp ${THEFILES})
