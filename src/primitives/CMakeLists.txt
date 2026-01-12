file(GLOB_RECURSE FILES_OPR_PRIM "operations/*.cpp")
file(GLOB_RECURSE FILES_GLB "global/*.cpp")
file(GLOB_RECURSE FILES_IMG "file_processing/charge/*.cpp")
file(GLOB_RECURSE FILES_REA "file_processing/readers/*.cpp")
file(GLOB_RECURSE FILES_GLY "glyph_system/*.cpp")
file(GLOB_RECURSE FILES_BOL "bool_matrix/*.cpp")
file(GLOB_RECURSE FILES_RAS "rasterizer/*.cpp")

add_library (objsrc_primitives OBJECT)
target_include_directories (objsrc_primitives PUBLIC ${CMAKE_SOURCE_DIR}/headers)
target_sources (objsrc_primitives PRIVATE 
  ${FILES_OPR_PRIM} ${FILES_IMG} ${FILES_REA} ${FILES_GLB} ${FILES_GLY} ${FILES_BOL} ${FILES_RAS}
)
