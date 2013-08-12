﻿#ifndef CLIPPER_H
#define CLIPPER_H

#include <QSettings>
#include <QWidget>
#include <QClipboard>
#include <QKeySequence>
#include <QDir>
#include <QSystemTrayIcon>
#include <QApplication>
#include <QMessageBox>
#include <QMenu>
#include <QLabel>
#include <QTextDocument>
#include <QMap>
#include <QPixmap>
#include <QScreen>
#include <QDesktopWidget>
#include <QBuffer>
#include <QDebug>
#include <QTime>
#include <QAction>
#include "ui_clipper.h"

#include "hotkey/qxtglobalshortcut.h"
#include "hotkey/qhotkeyedit.h"
#include "clipperapis.h"
#include "tnyczoptions.h"

namespace Ui {
class Clipper;
}

class Clipper : public QWidget
{
    Q_OBJECT
    
public:
    explicit Clipper(QWidget *parent = 0);
    ~Clipper();

public slots:
    void linkShorten();
    void linkToClipboard(QString link);
    void tnyczPublish();
    void updateHistory();
    void onChangeHotkeyButtonClicked(int id);
    void changeHotkey(QString hotkey);
    void saveSettings();
    void onTrayIconClicked(QSystemTrayIcon::ActivationReason reason);
    void makeScreenshot();
    void reloadSettings();
    void historyItemToClipboard(QListWidgetItem* item);
    void makeQRCode();
    void QRCodeReady(QPixmap *qrCode);
    void updateSettingsGUI();
    void toggleMulticopy();
    void updateMulticopyStore();

private:
    Ui::Clipper *ui;
    QSystemTrayIcon *tray;
    QClipboard *clipboard;
    ClipperAPIs *api;
    QButtonGroup shortcutButtons;
    int button_id;
    QSettings *settings;
    QIcon appIcon;
    QHotkeyEdit *hotkeyEditWindow;
    TnyczOptions *tnyczOptionsWindow;
    bool multicopyEnabled;

    QMap<QString, QString> shortcuts;
    QMap<QString, bool> general;
    QxtGlobalShortcut *linkShortenShortcut;
    QxtGlobalShortcut *tnyczPublishShortcut;
    QxtGlobalShortcut *screenshotShortcut;
    QxtGlobalShortcut *makeQRCodeShortcut;
    QxtGlobalShortcut *toggleMulticopyShortcut;
    bool hotkeysInit = true;

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // CLIPPER_H
