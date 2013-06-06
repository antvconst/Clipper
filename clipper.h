#ifndef CLIPPER_H
#define CLIPPER_H

#include <QSettings>
#include <QWidget>
#include <QClipboard>
#include <QKeySequence>
#include <QSystemTrayIcon>
#include <QApplication>
#include <QMessageBox>
#include <QMenu>
#include <QMap>
#include <QPixmap>
#include <QDesktopWidget>
#include <QBuffer>
#include <QDebug>
#include <QTime>
#include <QAction>
#include "ui_clipper.h"

#include "hotkey/qhotkeyedit.h"
#include "clipperapis.h"
#include "tnyczoptions.h"
#include "qxt/qxtglobalshortcut.h"

namespace Ui {
class Clipper;
}

class Clipper : public QWidget
{
    Q_OBJECT
    
public:
    explicit Clipper(QWidget *parent = 0);
    ~Clipper();

private slots:
    void onLinkShortenShortcutActivated();
    void linkToClipboard(QString link);
    void onTnyczPublishShortcutActivated();
    void updateHistory();
    void onChangeHotkeyButtonClicked(int id);
    void changeHotkey(QString hotkey);
    void saveSettings();
    void onTrayIconClicked(QSystemTrayIcon::ActivationReason reason);
    void makeScreenshot();
    void initHotkeys();
    void historyItemToClipboard(QListWidgetItem* item);

private:
    Ui::Clipper *ui;
    QSystemTrayIcon *tray;
    QClipboard *clipboard;
    ClipperAPIs *api;
    QButtonGroup shortcutButtons;
    int button_id;
    QSettings *settings;

    QMap<QString, QString> shortcuts;
    QxtGlobalShortcut *linkShortenShortcut;
    QxtGlobalShortcut *tnyczPublishShortcut;
    QxtGlobalShortcut *screenshotShortcut;
    bool hotkeysInit = true;
};

#endif // CLIPPER_H
