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

unix|win32: LIBS += -lfann

unix|win32: LIBS += -ldoublefann


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
