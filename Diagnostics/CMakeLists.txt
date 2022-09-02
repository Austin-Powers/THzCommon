set(PROJECTNAME THzCommon.Diagnostics)

add_Library(${PROJECTNAME} STATIC
	HexView.cpp
	HexView.h
	StopWatch.cpp
	StopWatch.h
)

target_link_libraries(${PROJECTNAME} PUBLIC
	THzCommon.Utility
)
