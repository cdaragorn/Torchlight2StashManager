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
    torchlight2stashconverter.cpp \
    optionkeys.cpp \
    torchlight2stash.cpp \
    customlistwidget.cpp \
    torchlight2item.cpp \
    infinitestashstandarditemmodel.cpp \
    infinitestashtreeview.cpp \
    groupmanagertreeview.cpp \
    groupmanagerstandarditemmodel.cpp \
    maintabpageform.cpp \
    settingstabpageform.cpp \
    managegroupstabpageform.cpp \
    stashitemstable.cpp

HEADERS  += mainwindow.h \
    sharedstashlistwidget.h \
    optioncollection.h \
    sqlitetable.h \
    groupstable.h \
    addgroupdialog.h \
    torchlight2stashconverter.h \
    optionkeys.h \
    torchlight2stash.h \
    tools.h \
    customlistwidget.h \
    torchlight2item.h \
    infinitestashstandarditemmodel.h \
    infinitestashtreeview.h \
    groupmanagertreeview.h \
    groupmanagerstandarditemmodel.h \
    maintabpageform.h \
    settingstabpageform.h \
    managegroupstabpageform.h \
    stashitemstable.h

FORMS    += mainwindow.ui \
    maintabpageform.ui \
    settingstabpageform.ui \
    managegroupstabpageform.ui

RESOURCES += \
    images.qrc

QMAKE_LFLAGS += -static-libgcc
