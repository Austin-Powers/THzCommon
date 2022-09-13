set(PROJECTNAME THzCommon.Tests)

add_executable(${PROJECTNAME}
	converter/base64.cpp
	converter/huffmancoder.cpp
	converter/huffmancommons.cpp
	logging.cpp
	math/inrange.cpp
	math/point.cpp
	math/rectangle.cpp
	structures/queue.cpp
	structures/stack.cpp
	utility/bitbuffer.cpp
	utility/byteorder.cpp
	utility/fstreamhelpers.cpp
)

target_include_directories(${PROJECTNAME} PUBLIC
	${PROJECT_SOURCE_DIR}
)

target_link_libraries(${PROJECTNAME} PUBLIC
	THzCommon
	gtest_main
)

target_compile_definitions(${PROJECTNAME} PRIVATE
	HUFFMANTESTFILEPATH="${PROJECT_SOURCE_DIR}/test/converter/huffmancoder.cpp"
)

include(GoogleTest)
gtest_discover_tests(${PROJECTNAME})
