file(GLOB_RECURSE FILES_TEX "texture/*.cpp")
file(GLOB_RECURSE FILES_PHY "physical/*.cpp")
file(GLOB_RECURSE FILES_PRI "primitives/*.cpp")

add_library (objsrc_concepts OBJECT)
target_include_directories (objsrc_concepts PUBLIC ${CMAKE_SOURCE_DIR}/headers)
target_sources (objsrc_concepts PRIVATE ${FILES_PRI} ${FILES_PHY} ${FILES_TEX})
