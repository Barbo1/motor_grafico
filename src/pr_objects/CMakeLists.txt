file(GLOB_RECURSE FILES_CIR "circle/*.cpp")
file(GLOB_RECURSE FILES_LIN "line/*.cpp")
file(GLOB_RECURSE FILES_PAR "particle/*.cpp")
file(GLOB_RECURSE FILES_PROJ "projectiles/*.cpp")
file(GLOB_RECURSE FILES_SQU "square/*.cpp")
file(GLOB_RECURSE FILES_NDG "nedge/*.cpp")

add_library (objsrc_pr_objects OBJECT)
target_include_directories (objsrc_pr_objects PUBLIC ${CMAKE_SOURCE_DIR}/headers)
target_sources (objsrc_pr_objects PRIVATE 
  ${FILES_CIR} ${FILES_LIN} ${FILES_PAR} ${FILES_SQU} ${FILES_NDG} ${FILES_PROJ}
)
