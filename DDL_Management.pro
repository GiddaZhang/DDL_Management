#-------------------------------------------------
#
# Project created by QtCreator 2022-02-21T13:36:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DDL_Management
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    button_delete.cpp \
    button_next.cpp \
    button_prev.cpp \
    workingfile.cpp \
    description.cpp \
    button.cpp \
    button_new.cpp \
    ddl_block.cpp \
        main.cpp \
        mainwindow.cpp \
    ddl.cpp \
    axispainter.cpp \
    primarymenu.cpp \
    workingfilelistitem.cpp \
    notelistitem.cpp

HEADERS += \
    button_delete.h \
    button_next.h \
    button_prev.h \
    workingfile.h \
    description.h \
    button.h \
    button_new.h \
    ddl_block.h \
        mainwindow.h \
    ddl.h \
    axispainter.h \
    primarymenu.h \
    workingfilelistitem.h \
    notelistitem.h

FORMS += \
        mainwindow.ui

RESOURCES += \
    resource/qss.qrc
