file(GLOB_RECURSE FILES_PR_SQUARE "square/*.cpp")
file(GLOB_RECURSE FILES_PR_LINE "line/*.cpp")
file(GLOB_RECURSE FILES_PR_CIRCLE "circle/*.cpp")
file(GLOB_RECURSE FILES_PR_NEDGE "nedge/*.cpp")

add_library (objsrc_pr_objects OBJECT)
target_include_directories (objsrc_pr_objects PUBLIC ${CMAKE_SOURCE_DIR}/headers)
target_sources (objsrc_pr_objects PRIVATE ${FILES_PR_SQUARE} ${FILES_PR_CIRCLE} ${FILES_PR_NEDGE} ${FILES_PR_LINE})
