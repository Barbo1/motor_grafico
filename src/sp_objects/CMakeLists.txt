file(GLOB_RECURSE FILES_SR_IMP "impulse/*.cpp")

add_library (objsrc_sp_objects OBJECT)
target_include_directories (objsrc_sp_objects PUBLIC ${CMAKE_SOURCE_DIR}/headers)
target_sources (objsrc_sp_objects PRIVATE ${FILES_SR_IMP})
