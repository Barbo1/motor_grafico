file(GLOB_RECURSE FILES_OPR_PRIM "operations/*.cpp")
file(GLOB_RECURSE FILES_VEC_PRIM "vectors/*.cpp")

add_library (objsrc_primitives OBJECT)
target_include_directories (objsrc_primitives PUBLIC ${CMAKE_SOURCE_DIR}/headers)
target_sources (objsrc_primitives PRIVATE ${FILES_VEC_PRIM} ${FILES_OPR_PRIM})
