#-------------------------------------------------
#
# Project created by QtCreator 2016-04-16T15:01:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG +=C++11

TARGET = CAD
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    hgedge.cpp \
    hgvertex.cpp \
    hgraph.cpp

HEADERS  += mainwindow.h \
    hgedge.h \
    hgvertex.h \
    hgraph.h

FORMS    += mainwindow.ui
