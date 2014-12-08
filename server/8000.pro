#-------------------------------------------------
#
# Project created by QtCreator 2014-12-07T01:47:37
#
#-------------------------------------------------

QT       += core websockets

QT       -= gui

TARGET = server
CONFIG   += console c++11
CONFIG   -= app_bundle

QMAKE_CXXFLAGS += -g -O2

TEMPLATE = app


SOURCES += main.cpp \
    server.cpp \
    gameworld.cpp \
    function.cpp

HEADERS += \
    server.h \
    gameworld.h \
    player.h \
    function.h \
    bullet.h
