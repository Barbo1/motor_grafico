add_executable (fin ./pruebas_nedge/prueba_cuadrado_resolve_collision.cpp)
target_include_directories (fin PRIVATE ${CMAKE_SOURCE_DIR}/headers)
target_link_libraries (fin PRIVATE 
  objsrc_primitives
  objsrc_concepts
  objsrc_pr_objects
  objsrc_sp_objects
  SDL2::SDL2
)
