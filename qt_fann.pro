#-------------------------------------------------
#
# Project created by QtCreator 2011-12-21T12:23:15
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = qt_fann
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += -I/usr/local/include

LIBS += -L/usr/local/lib
LIBS += -lfann
LIBS += -lm


SOURCES += main.cpp \
    eegmarkerfile.cpp \
    raweegcapture.cpp \
    textdatafile.cpp \
    fanntestrunner.cpp \
    progressindicator.cpp

HEADERS += \
    eegmarkerfile.h \
    raweegcapture.h \
    textdatafile.h \
    fanntestrunner.h \
    progressindicator.h
