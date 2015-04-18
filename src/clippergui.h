#ifndef CLIPPERGUI_H
#define CLIPPERGUI_H

#include "clippercore.h"
#include "clippersettings.h"

#include "libs/qhotkeyedit/qhotkeyedit.h"

#include "libs/imagecropwidget/imagecropwidget.h"

#include <memory>

#include <QWidget>
#include <QSystemTrayIcon>
#include <QApplication>
#include <QMenu>
#include <QLabel>
#include <QMap>
#include <QPixmap>
#include <QFileDialog>
#include <QTime>
#include <QAbstractButton>
#include <QAction>
#include "ui_clipper.h"

#include <QDebug>

class ClipperCore;

namespace Ui {
class Clipper;
}

class ClipperGUI : public QWidget
{
    Q_OBJECT
    
public:
    explicit ClipperGUI(ClipperCore* core, QWidget *parent = 0);
    ~ClipperGUI();

    enum MessageType {
        Error = 0,
        Notification,
        CriticalError
    };

    ImageCropWidget *imageCrop;

public slots:
    void updateHistory(QString text);
    void saveSettings();
    void changeHotkey(QString hotkey);
    void processTrayIconClick(QSystemTrayIcon::ActivationReason reason);
    void syncSettings();
    void showTrayMessage(MessageType messageType, QString message);
    void displayQrCode(std::shared_ptr<QPixmap> qrCode);
    bool cropImage(std::shared_ptr<QPixmap> image);


private:
    Ui::Clipper *ui;
    QSystemTrayIcon *tray;
    QButtonGroup shortcutButtons;
    int hotkeyUnderChangeId;
    QIcon appIcon;
    QHotkeyEdit *hotkeyEditWindow;
    ClipperCore *core;
    ClipperSettings *settings;

protected:
    void closeEvent(QCloseEvent *event);
};

#endif
