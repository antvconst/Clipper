#-------------------------------------------------
#
# Project created by QtCreator 2013-05-31T21:12:00
#
#-------------------------------------------------

QT       += core gui network widgets

TARGET = Clipper

TEMPLATE = app

unix {
target.path = /usr/bin/
INSTALLS += target
}
win32{
RC_FILE = icon.rc
}

SOURCES += main.cpp\
           clipper.cpp \
           libs/json/json.cpp \
    clipperapis.cpp \
    tnyczoptions.cpp \
    libs/qhotkeyedit/qhotkeyedit.cpp \
    imageselectwidget.cpp

HEADERS  += clipper.h \
    libs/json/json.h \
    clipperapis.h \
    tnyczoptions.h \
    libs/qhotkeyedit/qhotkeyedit.h \
    imageselectwidget.h

RESOURCES += \
    Clipper.qrc

FORMS += \
    tnyczoptions.ui \
    clipper.ui \
    libs/qhotkeyedit/qkeyedit.ui

QMAKE_CXXFLAGS += -std=c++11

unix|win32: LIBS += -L$$PWD/libs/UGlobalHotkey/ -lUGlobalHotkey

INCLUDEPATH += $$PWD/libs/UGlobalHotkey
DEPENDPATH += $$PWD/libs/UGlobalHotkey
