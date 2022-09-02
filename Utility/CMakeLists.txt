set(PROJECTNAME THzCommon.Utility)

add_library(${PROJECTNAME} STATIC
	BitBuffer.cpp
	BitBuffer.h
	Concepts.hpp
	FStreamHelpers.h
	SpanHelpers.h
	Time.cpp
	Time.h
)

target_include_directories(${PROJECTNAME} PUBLIC
	${PROJECT_SOURCE_DIR}
)

target_link_libraries(${PROJECTNAME} PUBLIC
	Microsoft.GSL::GSL
	THzCommon.Logging
)
