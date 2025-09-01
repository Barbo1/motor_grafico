file(GLOB_RECURSE FILES_TEX "texture/*.cpp")
file(GLOB_RECURSE FILES_PHY "physical/*.cpp")
file(GLOB_RECURSE FILES_PRI "primitives/*.cpp")
file(GLOB_RECURSE FILES_IMG "image_processing/charge/*.cpp")

add_library (objsrc_concepts OBJECT)
target_include_directories (objsrc_concepts PUBLIC ${CMAKE_SOURCE_DIR}/headers)
target_sources (objsrc_concepts PRIVATE ${FILES_TEX} ${FILES_PRI} ${FILES_PHY} ${FILES_IMG})
