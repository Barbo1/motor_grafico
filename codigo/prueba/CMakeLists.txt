add_executable (fin pruebas.cpp)
target_include_directories (fin PRIVATE ${CMAKE_SOURCE_DIR}/headers)
target_sources (fin PRIVATE 
  $<TARGET_OBJECTS:objsrc_concepts> 
  $<TARGET_OBJECTS:objsrc_pr_objects>
  $<TARGET_OBJECTS:objsrc_primitives>
  $<TARGET_OBJECTS:objsrc_sp_objects>
)
target_link_libraries (fin PRIVATE SDL2::SDL2)
