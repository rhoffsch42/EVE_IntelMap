#-------------------------------------------------
#
# Project created by QtCreator 2016-12-23T22:26:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IntelMap
TEMPLATE = app


QT += widgets
QT += core gui multimedia

SOURCES += \
    main.cpp \
    intel.cpp \
    data.cpp \
    scene.cpp \
    system.cpp \
    link.cpp \
    tracker.cpp \
    visualalert.cpp

HEADERS += \
    intel.h \
    data.h \
    scene.h \
    system.h \
    link.h \
    tracker.h \
    visualalert.h

CONFIG += release
