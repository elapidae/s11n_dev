TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    s11n_decoder.cpp \
    s11n_reader.cpp \
    s11n_serial.cpp \
    s11n_size.cpp \
    s11n_encoder.cpp \
    impl/str_view.cpp \
    s11n_writer.cpp

HEADERS += \
    impl/container_helper.h \
    impl/description.h \
    impl/endian.h \
    impl/name_of_type.h \
    impl/signature.h \
    impl/crc.h \
    impl/std_string.h \
    impl/std_void_t.h \
    impl/tuple_helper.h \
    impl/type_spec.h \
    impl/writer.h \
    s11n.h \
    s11n_decoder.h \
    s11n_name_of_type.h \
    s11n_reader.h \
    s11n_serial.h \
    impl/str_view.h \
    impl/name_of_type_from_pf.h \
    s11n_size.h \
    s11n_encoder.h \
    s11n_writer.h


VLIBS_DIR = $$PWD/../../vlibs
include( $$VLIBS_DIR/vlog/vlog.pri )
include( $$VLIBS_DIR/vstring/vstring.pri )
