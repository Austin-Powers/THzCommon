set(PROJECTNAME THzCommon.Tests)

add_executable(
	${PROJECTNAME}
	Utility/BitBuffer.cpp
	Logging.cpp
)

target_include_directories(${PROJECTNAME} PUBLIC
	${PROJECT_SOURCE_DIR}
)

target_link_libraries(${PROJECTNAME} PUBLIC
	Utility
	Logging
	gtest_main
)

include(GoogleTest)
gtest_discover_tests(${PROJECTNAME})
