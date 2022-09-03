set(PROJECTNAME THzCommon.Tests)

add_executable(${PROJECTNAME}
	Logging.cpp
	Math/MinMax.cpp
	Math/Point.cpp
	Utility/BitBuffer.cpp
	Utility/FStreamHelpers.cpp
)

target_include_directories(${PROJECTNAME} PUBLIC
	${PROJECT_SOURCE_DIR}
)

target_link_libraries(${PROJECTNAME} PUBLIC
	THzCommon.Logging
	THzCommon.Math
	THzCommon.Utility
	gtest_main
)

include(GoogleTest)
gtest_discover_tests(${PROJECTNAME})
