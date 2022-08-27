set(PROJECTNAME THzCommon.Logging)

add_library(
	${PROJECTNAME}
	Logging.cpp
	Logging.h
	Logging.hpp
)

target_link_libraries(${PROJECTNAME} PUBLIC
	Microsoft.GSL::GSL
)
