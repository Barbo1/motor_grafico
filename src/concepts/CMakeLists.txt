file(GLOB_RECURSE FILES_TEX "visualizer/*.cpp")
file(GLOB_RECURSE FILES_IMF "image_modifier/*.cpp")
file(GLOB_RECURSE FILES_PHY "physical/*.cpp")
file(GLOB_RECURSE FILES_COL "collition/*.cpp")
file(GLOB_RECURSE FILES_LGT "lights/*.cpp")

add_library (objsrc_concepts OBJECT)
target_include_directories (objsrc_concepts PUBLIC ${CMAKE_SOURCE_DIR}/headers)
target_sources (objsrc_concepts PRIVATE 
  ${FILES_TEX} ${FILES_PHY} ${FILES_IMF} ${FILES_COL} ${FILES_LGT}
)
