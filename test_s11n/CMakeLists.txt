cmake_minimum_required(VERSION 2.8)

add_definitions( -std=c++11 )

project( test_s11n )

find_library( GTEST_LIBRARY      NAMES libgtest.a      PATHS ${EXT_LIBS_PATH} )
find_library( GTEST_MAIN_LIBRARY NAMES libgtest_main.a PATHS ${EXT_LIBS_PATH} )

set( S11N_DIR "${CMAKE_CURRENT_SOURCE_DIR}/.." )
include( "${S11N_DIR}/s11n.cmake" )

add_executable( ${PROJECT_NAME}
                ${S11N_HEADERS}
                main.cpp
                tests_preparing.h
                test_plains.h
                test_signature.h
                test_encoder.h
                test_metaargs.h
                test_name_of_type.h
                test_container_names.h )


target_link_libraries( ${PROJECT_NAME}
                       ${GTEST_LIBRARY}
                       ${GTEST_MAIN_LIBRARY}
                       -lgtest
                       -lpthread
                     )
