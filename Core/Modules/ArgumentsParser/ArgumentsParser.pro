#-------------------------------------------------
#
# Project created by QtCreator 2014-04-17T17:52:15
#
#-------------------------------------------------

QT       -= gui

TARGET = ArgumentsParser
TEMPLATE = lib

DEFINES += ARGUMENTSPARSER_LIBRARY

SOURCES += argumentsparser.cpp

HEADERS += argumentsparser.h\
        argumentsparser_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
