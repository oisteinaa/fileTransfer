#-------------------------------------------------
#
# Project created by QtCreator 2016-03-16T12:37:58
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = fileTransferServer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    tcpserver.cpp

HEADERS += \
    tcpserver.h
