cmake_minimum_required (VERSION 3.8)

project(ion)

function(app NAME IS_WIN_APP)
	set(${NAME}_SOURCES "")
	file(GLOB ${NAME}_SOURCES CONFIGURE_DEPENDS "src/${NAME}/*.cpp")
	
	foreach(ARG IN LISTS ARGN)
		file(GLOB RECURSIVE_SOURCES_${NAME}_${ARG} CONFIGURE_DEPENDS "src/${ARG}/*.cpp")
		foreach(FILE IN LISTS RECURSIVE_SOURCES_${NAME}_${ARG})
			set(${NAME}_SOURCES ${${NAME}_SOURCES} ${FILE})
		endforeach()
	endforeach()

	if(${IS_WIN_APP})
		add_executable (${NAME} WIN32 ${${NAME}_SOURCES})
	else()
		add_executable (${NAME} ${${NAME}_SOURCES})
	endif()

	target_precompile_headers(${NAME} PUBLIC include/pch.hpp)
	set_property(TARGET ${NAME} PROPERTY CXX_STANDARD 23)
	include_directories(${NAME} include)
endfunction()

if(MSVC)
	add_definitions(-DUNICODE -D_UNICODE)
	add_compile_options(/W4 /WX)
else()
	add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

app(launcher false utils)
app(test-game true tasky ecs engine utils)
app(perf-tests tasky ecs engine utils)