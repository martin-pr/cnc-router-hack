cmake_minimum_required (VERSION 2.6)
project (PLOT)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

find_library(LIBSERIAL NAMES serial)
find_library(LIBSERIALBUF NAMES serial)

set(ROUTER_SOURCES
	main.cpp
	command.cpp
	document.cpp
)

add_executable (router ${ROUTER_SOURCES})
target_link_libraries(router ${LIBSERIAL})
