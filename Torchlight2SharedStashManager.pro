#-------------------------------------------------
#
# Project created by QtCreator 2013-03-02T09:21:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets xml sql

TARGET = Torchlight2SharedStashManager
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    sharedstashlistwidget.cpp \
    optioncollection.cpp \
    sqlitetable.cpp \
    groupstable.cpp \
    addgroupdialog.cpp \
    stashtogroupmappingtable.cpp \
    stashestable.cpp \
    settingstabpage.cpp \
    torchlight2stashconverter.cpp \
    optionkeys.cpp \
    torchlight2stash.cpp \
    maintabpage.cpp \
    infinitestashitemslistwidget.cpp

HEADERS  += mainwindow.h \
    sharedstashlistwidget.h \
    optioncollection.h \
    sqlitetable.h \
    groupstable.h \
    addgroupdialog.h \
    stashtogroupmappingtable.h \
    stashestable.h \
    settingstabpage.h \
    torchlight2stashconverter.h \
    optionkeys.h \
    torchlight2stash.h \
    tools.h \
    maintabpage.h \
    infinitestashitemslistwidget.h

FORMS    += mainwindow.ui

RESOURCES += \
    images.qrc

QMAKE_LFLAGS += -static-libgcc
