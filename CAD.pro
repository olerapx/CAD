#-------------------------------------------------
#
# Project created by QtCreator 2016-04-16T15:01:24
#
#-------------------------------------------------

QT       += core gui
QT += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG +=C++11

TARGET = CAD
TEMPLATE = app


SOURCES += main.cpp\
        gui/mainwindow.cpp \
    hgraph/hgedge.cpp \
    hgraph/hgvertex.cpp \
    hgraph/hgraph.cpp \
    chart/qcustomplot.cpp \
    chart/line.cpp \
    gui/datawindow.cpp

HEADERS  += gui/mainwindow.h \
    hgraph/hgedge.h \
    hgraph/hgvertex.h \
    hgraph/hgraph.h \
    chart/qcustomplot.h \
    chart/line.h \
   gui/datawindow.h

FORMS    += gui/mainwindow.ui \
    gui/datawindow.ui

DISTFILES +=
