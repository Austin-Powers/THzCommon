set(SOURCES
	Logging.cpp
	Logging.h
	Logging.hpp
)

setup_library("${SOURCES}")

get_project_name()
target_link_libraries(${PROJECTNAME} PUBLIC
	"GSL"
)
