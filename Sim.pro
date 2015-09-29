#-------------------------------------------------
#
# Project created by QtCreator 2015-05-04T14:50:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Sim
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    scene.cpp \
    game.cpp \
    helpwindow.cpp

HEADERS  += mainwindow.h \
    scene.h \
    game.h \
    helpwindow.h

FORMS    += mainwindow.ui \
    helpwindow.ui

RESOURCES += \
    pics/pics.qrc
