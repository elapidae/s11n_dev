#########################################################################################
##
##  VLIBS codebase
##
##  Authors:
##  Alexandre Gromtsev aka elapidae     elapidae@yandex.ru
##
##  GNU Lesser General Public License Usage
##  This file may be used under the terms of the GNU Lesser General Public License
##  version 3 as published by the Free Software Foundation and appearing in the file
##  LICENSE.LGPL3 included in the packaging of this file. Please review the following
##  information to ensure the GNU Lesser General Public License version 3 requirements
##  will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
#########################################################################################

#========================================================================================
# s11n.cmake
#========================================================================================


#========================================================================================

if ( NOT  S11N_INCLUDED )
    set ( S11N_INCLUDED TRUE )

    message( " === include s11n === ..." )

    include_directories( "${S11N_DIR}/")
    
    #set(HEADERS ${HEADERS} "${S11N_DIR}/s11n_.h")
    #set(SOURCES ${SOURCES} "${S11N_DIR}/s11n_.cpp")

    #set(HEADERS ${HEADERS} "${S11N_DIR}/s11n_.h")
    #set(SOURCES ${SOURCES} "${S11N_DIR}/s11n_.cpp")


    message( " === s11n included === " )

endif()
# vstring.cmake
#========================================================================================
