cmake_minimum_required(VERSION 3.14)
cmake_policy(VERSION 3.14)

if(NOT DEFINED PROJECT_NAME)
	set(THZ_COMMON_IS_ROOT True)
else()
	set(THZ_COMMON_IS_ROOT False)
endif()

project(THzCommon VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)
FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG        e2239ee6043f73722e7aa812a459f54a28552929 # release-1.11.0
)
FetchContent_Declare(
	gsl
	GIT_REPOSITORY https://github.com/microsoft/GSL.git
	GIT_TAG        a3534567187d2edc428efd3f13466ff75fe5805c # release-v4.0.0
)
FetchContent_MakeAvailable(gsl)

add_library(${PROJECT_NAME} STATIC
	src/converter/base64.cpp
	src/converter/huffmancoder.cpp
	src/converter/huffmancommons.cpp
	src/diagnostics/hexview.cpp
	src/diagnostics/stopwatch.cpp
	src/logging/logging.cpp
	src/math/point.cpp
	src/math/rectangle.cpp
	src/network/socketposix.cpp
	src/network/socketwin32.cpp
	src/utility/bitbuffer.cpp
	src/utility/byteorder.cpp
	src/utility/range2D.cpp
	src/utility/time.cpp

	include/THzCommon/converter/base64.hpp
	include/THzCommon/converter/huffmancoder.hpp
	include/THzCommon/converter/huffmancommons.hpp
	include/THzCommon/diagnostics/hexview.hpp
	include/THzCommon/diagnostics/stopwatch.hpp
	include/THzCommon/logging/logging.hpp
	include/THzCommon/math/bilinearInterpolation.hpp
	include/THzCommon/math/constants.hpp
	include/THzCommon/math/inrange.hpp
	include/THzCommon/math/matrix.hpp
	include/THzCommon/math/point.hpp
	include/THzCommon/math/rectangle.hpp
	include/THzCommon/memory/imemorypool.hpp
	include/THzCommon/network/socket.hpp
	include/THzCommon/network/socketposix.hpp
	include/THzCommon/network/socketwin32.hpp
	include/THzCommon/structures/octree.hpp
	include/THzCommon/structures/queue.hpp
	include/THzCommon/structures/stack.hpp
	include/THzCommon/utility/bitbuffer.hpp
	include/THzCommon/utility/byteorder.hpp
	include/THzCommon/utility/concepts.hpp
	include/THzCommon/utility/flipBuffer.hpp
	include/THzCommon/utility/fstreamhelpers.hpp
	include/THzCommon/utility/lineSequencer.hpp
	include/THzCommon/utility/range2D.hpp
	include/THzCommon/utility/spanhelpers.hpp
	include/THzCommon/utility/staticPImpl.hpp
	include/THzCommon/utility/time.hpp
	include/THzCommon/utility/workerThread.hpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
	${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PUBLIC
	Microsoft.GSL::GSL
)

if(WIN32)
	target_link_libraries(${PROJECT_NAME} PUBLIC
	wsock32
	ws2_32
	)
endif()

if(THZ_COMMON_IS_ROOT)
	# For Windows: Prevent overriding the parent project's compiler/linker settings
	# Not needed here as there will be no parent project, but its good to remember this command
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
	FetchContent_MakeAvailable(googletest)
	enable_testing()
	add_subdirectory(test)
endif()
