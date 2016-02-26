#-------------------------------------------------
#
# Project created by QtCreator 2015-06-17T11:45:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RayTracer
TEMPLATE = app
QMAKE_CXXFLAGS += -O3 -std=c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    camera.cpp \
    scene.cpp \
    SimpleObject.cpp \
    Vec3f.cpp \
    geometryelement.cpp \
    mythread.cpp \
    kdtree.cpp \
    constant.cpp

HEADERS  += mainwindow.h \
    camera.h \
    scene.h \
    SimpleObject.h \
    Vec3f.h \
    geometryelement.h \
    mythread.h \
    kdtree.h \
    constant.h

FORMS    += mainwindow.ui

RESOURCES += \
    ../resource/texture.qrc
