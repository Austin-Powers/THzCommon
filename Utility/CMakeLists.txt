set(PROJECTNAME THzCommon.Utility)

add_library(${PROJECTNAME} STATIC
	BitBuffer.cpp
	BitBuffer.h
	Concepts.hpp
	FStreamHelpers.h
	SpanHelpers.h
 "Concepts.hpp")

target_include_directories(${PROJECTNAME} PUBLIC
	${PROJECT_SOURCE_DIR}
)

target_link_libraries(${PROJECTNAME} PUBLIC
	Microsoft.GSL::GSL
	THzCommon.Logging
)
