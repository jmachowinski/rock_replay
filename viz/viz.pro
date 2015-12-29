#-------------------------------------------------
#
# Project created by QtCreator 2015-12-16T11:09:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = viz
TEMPLATE = app

FORMS += main.ui

SOURCES += main.cpp \
    replay_gui.cpp

HEADERS  += \
    replay_gui.h

RESOURCES += ressources.qrc

INCLUDEPATH += /usr/include/qwt-qt4 \
    $PWD../src/
LIBS += -L/usr/lib/ -lqwt-qt4
