cmake_minimum_required(VERSION 3.14)

project(THzCommon VERSION 1.0)

include(cmake/helper_functions.cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)
FetchContent_Declare(
	gsl
	GIT_REPOSITORY https://github.com/microsoft/GSL.git
	GIT_TAG        a3534567187d2edc428efd3f13466ff75fe5805c # release-v4.0.0
)
FetchContent_MakeAvailable(gsl)

add_subdirectory(Logging)
add_subdirectory(Tests)
