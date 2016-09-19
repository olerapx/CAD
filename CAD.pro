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
    hgraph/hgraph.cpp \
    chart/qcustomplot.cpp \
    chart/line.cpp \
    gui/datawindow.cpp \
    hgraph/hgraphexception.cpp \
    chart/linecolorpicker.cpp \
    hgraph/hedge.cpp \
    hgraph/hvertex.cpp \
    hgraph/hgraphworker.cpp

HEADERS  += gui/mainwindow.h \
    hgraph/hgraph.h \
    chart/qcustomplot.h \
    chart/line.h \
   gui/datawindow.h \
    hgraph/hgraphexception.h \
    chart/linecolorpicker.h \
    hgraph/hedge.h \
    hgraph/hvertex.h \
    hgraph/hgraphworker.h

FORMS    += gui/mainwindow.ui \
    gui/datawindow.ui

DISTFILES +=
