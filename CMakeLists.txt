cmake_minimum_required (VERSION 3.8)

project(ion)

function(app NAME)
	set(${NAME}_SOURCES "")
	file(GLOB ${NAME}_SOURCES CONFIGURE_DEPENDS "src/${NAME}/*.cpp")
	
	foreach(ARG IN LISTS ARGN)
		file(GLOB RECURSIVE_SOURCES_${NAME}_${ARG} CONFIGURE_DEPENDS "src/${ARG}/*.cpp")
		foreach(FILE IN LISTS RECURSIVE_SOURCES_${NAME}_${ARG})
			set(${NAME}_SOURCES ${${NAME}_SOURCES} ${FILE})
		endforeach()
	endforeach()

	add_executable (${NAME} ${${NAME}_SOURCES})
	target_precompile_headers(${NAME} PUBLIC include/pch.hpp)
	set_property(TARGET ${NAME} PROPERTY CXX_STANDARD 23)
	include_directories(${NAME} include)
endfunction()

app(launcher)
app(test-game tasky ecs engine)

app(perf-tests tasky)
