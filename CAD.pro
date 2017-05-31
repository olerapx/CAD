#-------------------------------------------------
#
# Project created by QtCreator 2016-04-16T15:01:24
#
#-------------------------------------------------

TARGET = CAD
TEMPLATE = app

QT += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

HEADERS  += \
    gui/mainwindow.h \
    gui/datawindow.h \
    chart/callout.h \
    hgraph/hgraph.h \
    hgraph/hgraphexception.h \
    hgraph/hedge.h \
    hgraph/hvertex.h \
    hgraph/hgraphworker.h

SOURCES += main.cpp \
    gui/mainwindow.cpp \
    gui/datawindow.cpp \
    chart/callout.cpp \
    hgraph/hgraph.cpp \
    hgraph/hgraphexception.cpp \
    hgraph/hedge.cpp \
    hgraph/hvertex.cpp \
    hgraph/hgraphworker.cpp

FORMS    += \
    gui/mainwindow.ui \
    gui/datawindow.ui
