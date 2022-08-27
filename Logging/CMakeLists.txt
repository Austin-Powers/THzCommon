set(PROJECTNAME THzCommon.Logging)

add_library(${PROJECTNAME} STATIC
	Logging.cpp
	Logging.h
	Logging.hpp
)

target_link_libraries(${PROJECTNAME} PUBLIC
	Microsoft.GSL::GSL
)
