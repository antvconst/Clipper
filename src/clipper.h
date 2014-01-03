#ifndef CLIPPER_H
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
#include <QDateTime>
#include <QScreen>
#include <QDesktopWidget>
#include <QBuffer>
#include <QDebug>
#include <QFileDialog>
#include <QTime>
#include <QAction>
#include "ui_clipper.h"

#include "libs/qhotkeyedit/qhotkeyedit.h"
#include "UGlobalHotkey/uglobalhotkeys.h"
#include "clipperapis.h"
#include "tnyczoptions.h"
#include "imageselectwidget.h"

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
    void makeFullScreenshot();
    void reloadSettings();
    void historyItemToClipboard(QListWidgetItem* item);
    void makeQRCode();
    void QRCodeReady(QPixmap *qrCode);
    void updateSettingsGUI();
    void toggleMulticopy();
    void updateMulticopyStore();
    void makePartialScreenshot();
    void saveScreenshotToFile(QPixmap *screenshot);
    void onHotkeyActivated(size_t id);
    inline QPixmap grabScreen()
    {
        QScreen *screen = QGuiApplication::primaryScreen();
        return screen->grabWindow(0);
    }
    inline QByteArray pixmapToByteArray(QPixmap *a)
    {
        QByteArray screenshotData;
        QBuffer buffer(&screenshotData);
        buffer.open(QIODevice::WriteOnly);
        a->save(&buffer, "PNG");
        return screenshotData;
    }
    inline QString getCurrentDatetime()
    {
        return QDateTime::currentDateTime().toString("dd.MM.yyyy hh.mm.ss");
    }

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
    QString screenshotPath;
    QString historyFilePath;
    QFile *historyFile = nullptr;
    UGlobalHotkeys *hotkeys;

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // CLIPPER_H
