TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

S11N_DIR = $$PWD/..
include( $$S11N_DIR/s11n.pri )

INCLUDEPATH += $$PWD
