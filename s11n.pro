TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    s11n_introspection.cpp \
    s11n_serial.cpp

HEADERS += \
    s11n_introspection.h \
    s11n_serial.h


VLIBS_DIR = $$PWD/../../vlibs
include( $$VLIBS_DIR/vlog/vlog.pri )
include( $$VLIBS_DIR/vstring/vstring.pri )
