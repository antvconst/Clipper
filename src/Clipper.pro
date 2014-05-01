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
desktop.path = /usr/share/applications

icon.files = misc/clipper.png
icon.path = /usr/share/icons

libs.files = libs/UGlobalHotkey/libUGlobalHotkey.*
libs.path = /usr/lib

INSTALLS += binary libs desktop icon
}

win32 {
RC_FILE = misc/icon.rc
}

SOURCES += \
    main.cpp \
    libs/json/json.cpp \
    tnyczoptions.cpp \
    libs/qhotkeyedit/qhotkeyedit.cpp \
    libs/imagecropwidget/imagecropwidget.cpp \
    clippercore.cpp \
    clippersettings.cpp \
    clippernetwork.cpp \
    clippergui.cpp

HEADERS  += \
    libs/json/json.h \
    tnyczoptions.h \
    libs/qhotkeyedit/qhotkeyedit.h \
    libs/imagecropwidget/imagecropwidget.h \
    clippercore.h \
    clippersettings.h \
    clippernetwork.h \
    clippergui.h

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
