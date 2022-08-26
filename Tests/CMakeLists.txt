set(SOURCES
	Logging.cpp
)

setup_executable("${SOURCES}")

get_project_name()
target_link_libraries(${PROJECTNAME} PUBLIC
	Logging
	gtest_main
)

include(GoogleTest)
gtest_discover_tests(${PROJECTNAME})
