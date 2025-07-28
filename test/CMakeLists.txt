set(TEST_FILES 
	configuration/configuration.cpp
	configuration/configurationbuilder.cpp
	configuration/configurationstorage.cpp
	converter/base64.cpp
	converter/huffmancoder.cpp
	converter/huffmancommons.cpp
	logging.cpp
	math/bilinearInterpolation.cpp
	math/inrange.cpp
	math/matrix.cpp
	math/point.cpp
	math/rectangle.cpp
	network/address.cpp
	network/tcpconnection.cpp
	network/tcpsocket.cpp
	network/udpsocket.cpp
	random/ant.cpp
	structures/octree.cpp
	structures/queue.cpp
	structures/stack.cpp
	utility/bitbuffer.cpp
	utility/byteorder.cpp
	utility/flipBuffer.cpp
	utility/fstreamhelpers.cpp
	utility/lineSequencer.cpp
	utility/range2D.cpp
	utility/result.cpp
	utility/spanhelpers.cpp
	utility/sptesthelper.cpp
	utility/sptesthelper.hpp
	utility/staticPImpl.cpp
	utility/stringviewhelpers.cpp
)
set(THZ_COMMON_TEST_FILES ${TEST_FILES} PARENT_SCOPE)

if(THZ_COMMON_IS_ROOT)
set(PROJECTNAME THzCommon.Tests)

add_executable(${PROJECTNAME} ${TEST_FILES})

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

add_test(NAME ${PROJECTNAME} COMMAND ${PROJECTNAME})
endif()
