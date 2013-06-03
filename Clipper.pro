#-------------------------------------------------
#
# Project created by QtCreator 2013-05-31T21:12:00
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Clipper

TEMPLATE = app

target.path = /usr/local/bin/
INSTALLS += target

SOURCES += main.cpp\
           clipper.cpp \
           json/json.cpp \
    clipperapis.cpp \
    tnyczoptions.cpp \
    hotkey/qhotkeyedit.cpp

HEADERS  += clipper.h \
    json/json.h \
    clipperapis.h \
    tnyczoptions.h \
    hotkey/qhotkeyedit.h

# Qxt
!macx:SOURCES += qxt/qxtglobalshortcut.cpp
!macx:HEADERS += qxt/qxtglobalshortcut_p.h \
    qxt/qxtglobalshortcut.h \
    qxt/qxtglobal.h
win32:SOURCES += qxt/qxtglobalshortcut_win.cpp
unix:!macx:SOURCES += qxt/qxtglobalshortcut_x11.cpp

RESOURCES += \
    Clipper.qrc

FORMS += \
    tnyczoptions.ui \
    clipper.ui \
    hotkey/qkeyedit.ui

QMAKE_CXXFLAGS += -std=c++11
