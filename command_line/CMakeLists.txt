cmake_minimum_required (VERSION 2.6)
project (PLOT)

find_library(LIBSERIAL NAMES serial)
find_library(LIBSERIALBUF NAMES serial)

find_library(LIBREADLINE NAMES readline)

EXEC_PROGRAM(ncurses5-config ARGS --libs OUTPUT_VARIABLE CURSES_LIBRARIES)

SET(ROUTER_SOURCES
	main.cpp
	command.cpp
	document.cpp
	interface.cpp
)

add_executable (router ${ROUTER_SOURCES})
target_link_libraries(router ${LIBSERIAL} ${CURSES_LIBRARIES} ${LIBREADLINE} menu)
