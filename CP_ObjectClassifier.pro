QT += core
QT -= gui

TARGET = CP_ObjectClassifier
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    utils.cpp

INCLUDEPATH += C:/Users/DAT/Desktop/opencv/build/include
LIBS += C:/Users/DAT/Desktop/opencv/cmake-debug/lib/*.a

HEADERS += \
    utils.h
