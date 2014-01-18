#-------------------------------------------------
#
# Project created by QtCreator 2013-05-31T21:12:00
#
#-------------------------------------------------

QT       += core gui network widgets

TARGET = clipper

TEMPLATE = app

unix {
binary.files = $$OUT_PWD/clipper
binary.path = /usr/bin

desktop.files = misc/Clipper.desktop
desktop.path = ~/.local/share/applications

icon.files = misc/clipper.png
icon.path = ~/.local/share/icons

libs.files = libs/UGlobalHotkey/libUGlobalHotkey.*
libs.path = /usr/lib

INSTALLS += binary libs desktop icon
}
win32{
RC_FILE = misc/icon.rc
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
