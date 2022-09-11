set(PROJECTNAME THzCommon.Tests)

add_executable(${PROJECTNAME}
	Converter/Base64.cpp
	Converter/HuffmanCoder.cpp
	Converter/HuffmanCommons.cpp
	Logging.cpp
	Math/InRange.cpp
	Math/MinMax.cpp
	Math/Point.cpp
	Math/Rectangle.cpp
	Utility/BitBuffer.cpp
	Utility/FStreamHelpers.cpp
)

target_include_directories(${PROJECTNAME} PUBLIC
	${PROJECT_SOURCE_DIR}
)

target_link_libraries(${PROJECTNAME} PUBLIC
	THzCommon.Converter
	THzCommon.Logging
	THzCommon.Math
	THzCommon.Utility
	gtest_main
)

target_compile_definitions(${PROJECTNAME} PRIVATE
	HUFFMANTESTFILEPATH="${PROJECT_SOURCE_DIR}/Tests/Converter/HuffmanCoder.cpp"
)

include(GoogleTest)
gtest_discover_tests(${PROJECTNAME})
