#-------------------------------------------------
#
# Project created by QtCreator 2013-05-31T21:12:00
#
#-------------------------------------------------

QT       += core gui network gui-private widgets

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
           json/json.cpp \
    clipperapis.cpp \
    tnyczoptions.cpp \
    hotkey/qhotkeyedit.cpp \

HEADERS  += clipper.h \
    json/json.h \
    clipperapis.h \
    tnyczoptions.h \
    hotkey/qhotkeyedit.h

RESOURCES += \
    Clipper.qrc

FORMS += \
    tnyczoptions.ui \
    clipper.ui \
    hotkey/qkeyedit.ui

QMAKE_CXXFLAGS += -std=c++11

# Qxt
!macx:SOURCES += hotkey/qxtglobalshortcut.cpp
!macx:HEADERS += \
    hotkey/qxtglobalshortcut.h \
    hotkey/qxtglobal.h
win32:SOURCES += hotkey/qxtglobalshortcut_win.cpp
unix:!macx:SOURCES += hotkey/qxtglobalshortcut_x11.cpp
unix:!macx:LIBS += -lX11
