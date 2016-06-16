QT += core
QT -= gui

TARGET = CP_ObjectClassifier
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

SOURCES += main.cpp \
    utils.cpp

INCLUDEPATH += C://Users//DAT//Desktop//opencv//build//include
LIBS += C://Users//DAT//Desktop//opencv//cmake-debug//lib//libopencv_calib3d_pch_dephelp.a
LIBS += C://Users//DAT//Desktop//opencv//cmake-debug//lib//libopencv_core2411.dll.a
LIBS += C://Users//DAT//Desktop//opencv//cmake-debug//lib//libopencv_features2d2411.dll.a
LIBS += C://Users//DAT//Desktop//opencv//cmake-debug//lib//libopencv_flann2411.dll.a
LIBS += C://Users//DAT//Desktop//opencv//cmake-debug//lib//libopencv_gpu2411.dll.a
LIBS += C://Users//DAT//Desktop//opencv//cmake-debug//lib//libopencv_highgui2411.dll.a
LIBS += C://Users//DAT//Desktop//opencv//cmake-debug//lib//libopencv_imgproc2411.dll.a
LIBS += C://Users//DAT//Desktop//opencv//cmake-debug//lib//libopencv_legacy2411.dll.a
LIBS += C://Users//DAT//Desktop//opencv//cmake-debug//lib//libopencv_ml2411.dll.a
LIBS += C://Users//DAT//Desktop//opencv//cmake-debug//lib//libopencv_nonfree2411.dll.a
LIBS += C://Users//DAT//Desktop//opencv//cmake-debug//lib//libopencv_objdetect2411.dll.a
LIBS += C://Users//DAT//Desktop//opencv//cmake-debug//lib//libopencv_ocl2411.dll.a
LIBS += C://Users//DAT//Desktop//opencv//cmake-debug//lib//libopencv_photo2411.dll.a

HEADERS += \
    utils.h
