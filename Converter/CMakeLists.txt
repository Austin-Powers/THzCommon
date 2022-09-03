set(PROJECTNAME THzCommon.Converter)

add_library(${PROJECTNAME} STATIC
	HuffmanCoder.cpp
	HuffmanCoder.h
	HuffmanCommons.cpp
	HuffmanCommons.h
)

target_include_directories(${PROJECTNAME} PUBLIC
	${PROJECT_SOURCE_DIR}
)

target_link_libraries(${PROJECTNAME} PUBLIC
	Microsoft.GSL::GSL
	THzCommon.Logging
	THzCommon.Utility
)