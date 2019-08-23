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
    
    set(S11N_HEADERS ${S11N_HEADERS} "${S11N_DIR}/impl/str_view.h")
    set(S11N_HEADERS ${S11N_HEADERS} "${S11N_DIR}/impl/std_void_t.h")
    set(S11N_HEADERS ${S11N_HEADERS} "${S11N_DIR}/impl/crc.h")
    set(S11N_HEADERS ${S11N_HEADERS} "${S11N_DIR}/impl/tuple_helper.h")
    set(S11N_HEADERS ${S11N_HEADERS} "${S11N_DIR}/impl/container_helper.h")
    set(S11N_HEADERS ${S11N_HEADERS} "${S11N_DIR}/impl/name_of_type_from_pf.h")

    set(S11N_HEADERS ${S11N_HEADERS} "${S11N_DIR}/impl/signature_1_name_of_type.h")
    set(S11N_HEADERS ${S11N_HEADERS} "${S11N_DIR}/impl/signature_2_description.h")
    set(S11N_HEADERS ${S11N_HEADERS} "${S11N_DIR}/impl/signature_3_serial_tuple.h")
    set(S11N_HEADERS ${S11N_HEADERS} "${S11N_DIR}/impl/signature.h")

    set(S11N_HEADERS ${S11N_HEADERS} "${S11N_DIR}/impl/std_string.h")

    set(S11N_HEADERS ${S11N_HEADERS} "${S11N_DIR}/s11n.h")

    #set(S11N_HEADERS ${S11N_HEADERS} "${S11N_DIR}/s11n_.h")
    #set(S11N_SOURCES ${S11N_SOURCES} "${S11N_DIR}/s11n_.cpp")


    message( " === s11n included === " )

endif()
# vstring.cmake
#========================================================================================
