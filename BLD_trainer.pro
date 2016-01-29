#-------------------------------------------------
#
# Project created by QtCreator 2016-01-02T11:48:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BLD_trainer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    generator.cpp \
    my_chains.cpp \
    probs.cpp \
    linewithtabs.cpp \
    tables_loader.cpp

HEADERS  += mainwindow.h \
    generator.h \
    my_chains.h \
    probs.h \
    linewithtabs.h \
    tables_loader.h

FORMS    += mainwindow.ui

CONFIG += c++11
