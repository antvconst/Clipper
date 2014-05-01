#ifndef CLIPPERCORE_H
#define CLIPPERCORE_H

#include "clippergui.h"
#include "clippersettings.h"
#include "clippernetwork.h"

#include "tnyczoptions.h"

#include "UGlobalHotkey/uglobalhotkeys.h"

#include <memory>

#include <QObject>
#include <QByteArray>
#include <QString>
#include <QClipboard>
#include <QBuffer>
#include <QGuiApplication>
#include <QScreen>
#include <QDateTime>

class ClipperGUI;

class ClipperCore : public QObject
{
    Q_OBJECT

public:
    explicit ClipperCore(QObject *parent = 0);

public slots:
    void putStringIntoClipboard(QString str);
    void processHotkeyActivation(size_t activatedHotkey);
    void updateSettings();

private:
    QClipboard *clipboard;
    QFile *historyFile = nullptr;

    ClipperSettings *settings;
    ClipperNetwork *network;
    ClipperGUI *gui;

    UGlobalHotkeys *hotkeys;
    QMap<QString, QMetaObject::Connection> connections;

    const QString ERROR_CLIPBOARD_IS_EMPTY = "Clipboard is empty";
    const QString NOTIFICATION_SCREENSHOT_UPLOADING = "Uploading screenshot, please wait..";
    const QString ERROR_SAVING_SCREENSHOT = "Error saving screenshot: %1";
    const QString NOTIFICATION_LINK_IS_READY = "Link is ready";
    const QString NOTIFICATION_QR_CODE_READY = "QR code is ready";

    void processFullScreenshot();
    void processPartialScreenshot();
    void processLinkShortening();
    void processQRCode();
    void processTextSharing();
    void savePixmapToFile(std::shared_ptr<QPixmap> pixmap);

    inline QString getClipboardText()
    {
        return clipboard->text();
    }

    inline QPixmap grabScreen()
    {
        QScreen *screen = QGuiApplication::primaryScreen();
        return screen->grabWindow(0);
    }

    inline QByteArray pixmapToByteArray(std::shared_ptr<QPixmap> a)
    {
        QByteArray screenshotData;
        QBuffer buffer(&screenshotData);
        buffer.open(QIODevice::WriteOnly);
        a->save(&buffer, "PNG");
        return screenshotData;
    }

    inline  QString getCurrentDatetime()
    {
        return QDateTime::currentDateTime().toString("dd.MM.yyyy hh.mm.ss");
    }
};

#endif // CLIPPERCORE_H
