QT += core
QT -= gui

TARGET = CP_ObjectClassifier
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    utils.cpp

INCLUDEPATH += D://opencv//opencv//build//include
LIBS += "D://opencv//opencv//builded-lib//lib//libopencv_calib3d_pch_dephelp.a"
LIBS += "D://opencv//opencv//builded-lib//lib//libopencv_core249.dll.a"
LIBS += "D://opencv//opencv//builded-lib//lib//libopencv_features2d249.dll.a"
LIBS += "D://opencv//opencv//builded-lib//lib//libopencv_flann249.dll.a"
LIBS += "D://opencv//opencv//builded-lib//lib//libopencv_gpu249.dll.a"
LIBS += "D://opencv//opencv//builded-lib//lib//libopencv_highgui249.dll.a"
LIBS += "D://opencv//opencv//builded-lib//lib//libopencv_imgproc249.dll.a"
LIBS += "D://opencv//opencv//builded-lib//lib//libopencv_legacy249.dll.a"
LIBS += "D://opencv//opencv//builded-lib//lib//libopencv_ml249.dll.a"
LIBS += "D://opencv//opencv//builded-lib//lib//libopencv_nonfree249.dll.a"
LIBS += "D://opencv//opencv//builded-lib//lib//libopencv_objdetect249.dll.a"
LIBS += "D://opencv//opencv//builded-lib//lib//libopencv_ocl249.dll.a"
LIBS += "D://opencv//opencv//builded-lib//lib//libopencv_photo249.dll.a"

HEADERS += \
    utils.h
