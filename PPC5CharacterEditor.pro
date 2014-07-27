#-------------------------------------------------
#
# Project created by QtCreator 2014-07-25T03:28:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PPC5CharacterEditor
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    timelinedelegate.cpp \
    timelinemodel.cpp \
    timelineheaderview.cpp

HEADERS  += mainwindow.h \
    timelinedelegate.h \
    timelinemodel.h \
    timelineheaderview.h

FORMS    += mainwindow.ui
