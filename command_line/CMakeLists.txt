cmake_minimum_required (VERSION 2.6)
project (PLOT)

find_library(LIBSERIAL NAMES serial)
find_library(LIBSERIALBUF NAMES serial)

SET(ROUTER_SOURCES
	main.cpp
	command.cpp
)

add_executable (router ${ROUTER_SOURCES})
target_link_libraries(router ${LIBSERIAL})
