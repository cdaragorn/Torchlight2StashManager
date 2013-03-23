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
    managegroupstabpage.cpp \
    customlistwidget.cpp \
    torchlight2item.cpp \
    infinitestashstandarditemmodel.cpp \
    infinitestashtreeview.cpp \
    groupmanagertreeview.cpp \
    groupmanagerstandarditemmodel.cpp \
    maintabpageform.cpp \
    settingstabpageform.cpp \
    managegroupstabpageform.cpp

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
    managegroupstabpage.h \
    customlistwidget.h \
    torchlight2item.h \
    infinitestashstandarditemmodel.h \
    infinitestashtreeview.h \
    groupmanagertreeview.h \
    groupmanagerstandarditemmodel.h \
    maintabpageform.h \
    settingstabpageform.h \
    managegroupstabpageform.h

FORMS    += mainwindow.ui \
    maintabpageform.ui \
    settingstabpageform.ui \
    managegroupstabpageform.ui

RESOURCES += \
    images.qrc

QMAKE_LFLAGS += -static-libgcc
