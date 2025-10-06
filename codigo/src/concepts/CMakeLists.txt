file(GLOB_RECURSE FILES_TEX "visualizer/*.cpp")
file(GLOB_RECURSE FILES_IMF "image_modifier/*.cpp")
file(GLOB_RECURSE FILES_PHY "physical/*.cpp")
file(GLOB_RECURSE FILES_OPR "operations/*.cpp")
file(GLOB_RECURSE FILES_COL "collition/*.cpp")
file(GLOB_RECURSE FILES_PRI "primitives/*.cpp")
file(GLOB_RECURSE FILES_IMG "file_processing/charge/*.cpp")

add_library (objsrc_concepts OBJECT)
target_include_directories (objsrc_concepts PUBLIC ${CMAKE_SOURCE_DIR}/headers)
target_sources (objsrc_concepts PRIVATE 
  ${FILES_TEX} ${FILES_PRI} ${FILES_PHY} 
  ${FILES_IMG} ${FILES_IMF} ${FILES_COL} 
  ${FILES_OPR}
)
