set(PROJECTNAME THzCommon.Logging)

add_library(${PROJECTNAME} STATIC
	Logging.cpp
	Logging.h
	Logging.hpp
)

target_include_directories(${PROJECTNAME} INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/")

target_link_libraries(${PROJECTNAME} PUBLIC
	Microsoft.GSL::GSL
)
