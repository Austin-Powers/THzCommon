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
	src/utility/bitbuffer.cpp
	src/utility/byteorder.cpp
	src/utility/time.cpp

	include/THzCommon/converter/base64.h
	include/THzCommon/converter/huffmancoder.h
	include/THzCommon/converter/huffmancommons.h
	include/THzCommon/diagnostics/hexview.h
	include/THzCommon/diagnostics/stopwatch.h
	include/THzCommon/logging/logging.h
	include/THzCommon/logging/logging.hpp
	include/THzCommon/math/constants.h
	include/THzCommon/math/inrange.h
	include/THzCommon/math/point.h
	include/THzCommon/math/rectangle.h
	include/THzCommon/memory/imemorypool.h
	include/THzCommon/structures/octree.h
	include/THzCommon/structures/queue.h
	include/THzCommon/structures/stack.h
	include/THzCommon/utility/bitbuffer.h
	include/THzCommon/utility/byteorder.h
	include/THzCommon/utility/concepts.h
	include/THzCommon/utility/flipBuffer.h
	include/THzCommon/utility/fstreamhelpers.h
	include/THzCommon/utility/spanhelpers.h
	include/THzCommon/utility/staticPImpl.h
	include/THzCommon/utility/time.h
)

target_include_directories(${PROJECT_NAME} PUBLIC
	${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PUBLIC
	Microsoft.GSL::GSL
)

if(THZ_COMMON_IS_ROOT)
	# For Windows: Prevent overriding the parent project's compiler/linker settings
	# Not needed here as there will be no parent project, but its good to remember this command
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
	FetchContent_MakeAvailable(googletest)
	enable_testing()
	add_subdirectory(test)
endif()
