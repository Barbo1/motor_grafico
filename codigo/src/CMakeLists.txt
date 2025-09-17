add_subdirectory (concepts)
add_subdirectory (pr_objects)
add_subdirectory (sp_objects)

add_library (objsrc OBJECT)
target_include_directories (objsrc PUBLIC ${CMAKE_SOURCE_DIR}/headers)
target_sources (objsrc PRIVATE 
  $<TARGET_OBJECTS:objsrc_concepts>
  $<TARGET_OBJECTS:objsrc_pr_objects>
)
