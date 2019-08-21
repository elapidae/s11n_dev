cmake_minimum_required(VERSION 2.8)

#set( VLIBS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../.." )
set( S11N_DIR "${CMAKE_CURRENT_SOURCE_DIR}/.." )
include( "${S11N_DIR}/s11n.cmake" )


project( test_s11n )


find_library( GTEST_LIBRARY      NAMES libgtest.a      PATHS ${EXT_LIBS_PATH} )
find_library( GTEST_MAIN_LIBRARY NAMES libgtest_main.a PATHS ${EXT_LIBS_PATH} )


add_executable( ${PROJECT_NAME} "main.cpp" ${HEADERS} ${SOURCES} )


target_link_libraries( ${PROJECT_NAME}
                       ${GTEST_LIBRARY}
                       ${GTEST_MAIN_LIBRARY}
                       ${V_LIBRARIES}
                       -lgtest
                       -lpthread
                     )
