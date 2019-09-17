TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt  # test without qt.

TARGET = form_as_one_file

SOURCES += form_as_one_file.cpp

#VLIBS_DIR = $$PWD/../..
#include( $$VLIBS_DIR/qmake/vcat.pri )
#include( $$VLIBS_DIR/qmake/gtest.pri )

INCLUDEPATH += $$PWD
