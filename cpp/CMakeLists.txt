cmake_minimum_required (VERSION 2.6) 
project (PLOT) 

find_library(LIBSERIAL NAMES serial)
find_library(LIBSERIALBUF NAMES serial)

find_package(Qt4 REQUIRED)

SET(ROUTER_SOURCES main.cpp)
SET(ROUTER_HEADERS )

QT4_WRAP_CPP(ROUTER_HEADERS_MOC ${ROUTER_HEADERS})

INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

add_executable (router ${ROUTER_SOURCES} ${ROUTER_HEADERS_MOC})
target_link_libraries(router ${LIBSERIAL} ${QT_LIBRARIES}) 
