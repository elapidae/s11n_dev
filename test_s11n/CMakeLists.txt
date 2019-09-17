cmake_minimum_required(VERSION 2.8)

#set( VLIBS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../.." )

add_definitions( -std=c++11 )

project( test_s11n )

set( HEADERS ${HEADERS} "test_main.h" )
#set( SOURCES ${SOURCES} "test_plains.сpp" )

find_library( GTEST_LIBRARY      NAMES libgtest.a      PATHS ${EXT_LIBS_PATH} )
find_library( GTEST_MAIN_LIBRARY NAMES libgtest_main.a PATHS ${EXT_LIBS_PATH} )


set( S11N_DIR "${CMAKE_CURRENT_SOURCE_DIR}/.." )
include( "${S11N_DIR}/s11n.cmake" )
set( HEADERS ${HEADERS} ${S11N_HEADERS} )
set( SOURCES ${SOURCES} ${S11N_SOURCES} )

add_executable( ${PROJECT_NAME} "main.cpp" ${HEADERS} ${SOURCES}
                test_signature.h
                test_encoder.h )


target_link_libraries( ${PROJECT_NAME}
                       ${GTEST_LIBRARY}
                       ${GTEST_MAIN_LIBRARY}
                       ${V_LIBRARIES}
                       -lgtest
                       -lpthread
                     )
