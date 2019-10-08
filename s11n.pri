#########################################################################################
##
##  VLIBS codebase, NIIAS
##
##  GNU Lesser General Public License Usage
##  This file may be used under the terms of the GNU Lesser General Public License
##  version 3 as published by the Free Software Foundation and appearing in the file
##  LICENSE.LGPL3 included in the packaging of this file. Please review the following
##  information to ensure the GNU Lesser General Public License version 3 requirements
##  will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
#########################################################################################
# s11n.pri

#========================================================================================
isEmpty(qi_s11n) {
    qi_s11n = 1;
    isEmpty(qi_not_print_pri_messages): message("=== s11n appended ===")

    isEmpty(S11N_DIR): error("Need S11N_DIR correct path.")

    INCLUDEPATH += $$S11N_DIR

    HEADERS += $$S11N_DIR/impl_s11n/container_helper.h
    HEADERS += $$S11N_DIR/impl_s11n/endian.h
    HEADERS += $$S11N_DIR/impl_s11n/metaargs_helper.h
    HEADERS += $$S11N_DIR/impl_s11n/crc.h
    HEADERS += $$S11N_DIR/impl_s11n/size.h
    HEADERS += $$S11N_DIR/impl_s11n/str_view.h
    HEADERS += $$S11N_DIR/impl_s11n/tuple_helper.h
    HEADERS += $$S11N_DIR/impl_s11n/type_spec.h
    HEADERS += $$S11N_DIR/impl_s11n/void_type.h

    HEADERS += $$S11N_DIR/impl_s11n/reader.h
    HEADERS += $$S11N_DIR/impl_s11n/writer.h

    HEADERS += $$S11N_DIR/impl_s11n/decode.h
    HEADERS += $$S11N_DIR/impl_s11n/encode.h

    HEADERS += $$S11N_DIR/impl_s11n/name_of_type_from_pf.h

    HEADERS += $$S11N_DIR/impl_s11n/signature_1_name_of_type.h
    HEADERS += $$S11N_DIR/impl_s11n/signature_2_metatype.h
    HEADERS += $$S11N_DIR/impl_s11n/signature_3_serial_tuple.h

    HEADERS += $$S11N_DIR/impl_s11n/std_list.h
    HEADERS += $$S11N_DIR/impl_s11n/std_string.h

    HEADERS += $$S11N_DIR/impl_s11n/signature.h

    HEADERS += $$S11N_DIR/s11n.h

    LIBS += -pthread -lgtest
}
# s11n.pri
#========================================================================================
