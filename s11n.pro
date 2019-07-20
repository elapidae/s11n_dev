TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    s11n_serial.cpp \
    s11n_size.cpp \
    impl/serializer.cpp \
    s11n_encoder.cpp

HEADERS += \
    s11n_serial.h \
    impl/str_view.h \
    impl/name_of_type_from_pf.h \
    s11n_size.h \
    impl/serializer.h \
    s11n_encoder.h


VLIBS_DIR = $$PWD/../../vlibs
include( $$VLIBS_DIR/vlog/vlog.pri )
include( $$VLIBS_DIR/vstring/vstring.pri )
