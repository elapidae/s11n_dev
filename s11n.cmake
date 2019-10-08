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

    set(S11N_HEADERS ${S11N_HEADERS} "${S11N_DIR}/impl_s11n/str_view.h")
    set(S11N_HEADERS ${S11N_HEADERS} "${S11N_DIR}/impl_s11n/void_type.h")
    set(S11N_HEADERS ${S11N_HEADERS} "${S11N_DIR}/impl_s11n/crc.h")
    set(S11N_HEADERS ${S11N_HEADERS} "${S11N_DIR}/impl_s11n/tuple_helper.h")
    set(S11N_HEADERS ${S11N_HEADERS} "${S11N_DIR}/impl_s11n/container_helper.h")
    set(S11N_HEADERS ${S11N_HEADERS} "${S11N_DIR}/impl_s11n/name_of_type_from_pf.h")
    set(S11N_HEADERS ${S11N_HEADERS} "${S11N_DIR}/impl_s11n/metaargs_helper.h")

    set(S11N_HEADERS ${S11N_HEADERS} "${S11N_DIR}/impl_s11n/signature_1_name_of_type.h")
    set(S11N_HEADERS ${S11N_HEADERS} "${S11N_DIR}/impl_s11n/signature_2_metatype.h")
    set(S11N_HEADERS ${S11N_HEADERS} "${S11N_DIR}/impl_s11n/signature_3_serial_tuple.h")
    set(S11N_HEADERS ${S11N_HEADERS} "${S11N_DIR}/impl_s11n/signature.h")

    set(S11N_HEADERS ${S11N_HEADERS} "${S11N_DIR}/impl_s11n/size.h")

    set(S11N_HEADERS ${S11N_HEADERS} "${S11N_DIR}/impl_s11n/reader.h")
    set(S11N_HEADERS ${S11N_HEADERS} "${S11N_DIR}/impl_s11n/writer.h")

    set(S11N_HEADERS ${S11N_HEADERS} "${S11N_DIR}/impl_s11n/encode.h")
    set(S11N_HEADERS ${S11N_HEADERS} "${S11N_DIR}/impl_s11n/decode.h")

    set(S11N_HEADERS ${S11N_HEADERS} "${S11N_DIR}/impl_s11n/std_string.h")
    set(S11N_HEADERS ${S11N_HEADERS} "${S11N_DIR}/impl_s11n/std_list.h")

    set(S11N_HEADERS ${S11N_HEADERS} "${S11N_DIR}/s11n.h")


    message( " === s11n included === " )

endif()
# vstring.cmake
#========================================================================================
