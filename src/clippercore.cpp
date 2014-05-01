#include "clippercore.h"

ClipperCore::ClipperCore(QObject *parent) :
    QObject(parent)
{
    settings = ClipperSettings::instance();
    network = ClipperNetwork::instance();
    hotkeys = new UGlobalHotkeys();
    gui = new ClipperGUI(this);
    clipboard = qApp->clipboard();

    connect(network, &ClipperNetwork::linkReady, [=](QString link) {
        putStringIntoClipboard(link);
        gui->showTrayMessage(ClipperGUI::Notification, NOTIFICATION_LINK_IS_READY);
    });

    connect(network, &ClipperNetwork::qrCodeReady, [=](std::shared_ptr<QPixmap> qrCode) {
        gui->displayQrCode(qrCode);
        gui->showTrayMessage(ClipperGUI::Notification, NOTIFICATION_QR_CODE_READY);
    });

    connect(network, &ClipperNetwork::failure, [=](QString message) {
        gui->showTrayMessage(ClipperGUI::Error, message);
    });

    connect(settings, &ClipperSettings::settingsChanged, [=]() {
        this->updateSettings();
    });

    this->updateSettings();
}

void ClipperCore::processFullScreenshot()
{
    std::shared_ptr<QPixmap> screenshot(new QPixmap(grabScreen()));
    std::shared_ptr<QByteArray> screenshotData(new QByteArray(pixmapToByteArray(screenshot)));
    network->uploadImage(screenshotData);
    gui->showTrayMessage(ClipperGUI::Notification, NOTIFICATION_SCREENSHOT_UPLOADING);
    if (settings->general["SaveScreenshots"])
        savePixmapToFile(screenshot);
}

void ClipperCore::processPartialScreenshot()
{
    std::shared_ptr<QPixmap> screenshot(new QPixmap(grabScreen()));

    if (!gui->cropImage(screenshot))
        return;

    else
    {
        std::shared_ptr<QByteArray> screenshotData(new QByteArray(pixmapToByteArray(screenshot)));
        network->uploadImage(screenshotData);
        gui->showTrayMessage(ClipperGUI::Notification, NOTIFICATION_SCREENSHOT_UPLOADING);
        if (settings->general["SaveScreenshots"])
            savePixmapToFile(screenshot);
        screenshot.reset();
    }
}

void ClipperCore::processLinkShortening()
{
    QString clipboardContent = getClipboardText();
    if (!clipboardContent.isEmpty())
        network->shortenLink(clipboardContent);
    else
        gui->showTrayMessage(ClipperGUI::Error, ERROR_CLIPBOARD_IS_EMPTY);
}

void ClipperCore::processQRCode()
{
    QString clipboardContent = getClipboardText();
    if (!clipboardContent.isEmpty())
        network->convertToQRCode(clipboardContent);
    else
        gui->showTrayMessage(ClipperGUI::Error, ERROR_CLIPBOARD_IS_EMPTY);
}

void ClipperCore::processTextSharing()
{
    if (gui->tnyczOptionsWindow->isVisible())
        return;

    QString clipboardContent = getClipboardText();
    if (!clipboardContent.isEmpty())
    {
        gui->tnyczOptionsWindow->show();
        gui->tnyczOptionsWindow->setPasteText(clipboardContent);

        connect(gui->tnyczOptionsWindow, SIGNAL(optionsReady(TnyczOptions::TextToPublish)),
                network, SLOT(shareText(TnyczOptions::TextToPublish)), Qt::UniqueConnection);
    }
    else
        gui->showTrayMessage(ClipperGUI::Error, ERROR_CLIPBOARD_IS_EMPTY);
}

void ClipperCore::savePixmapToFile(std::shared_ptr<QPixmap> pixmap)
{
    QFile file(settings->screenshotPath+"/"+getCurrentDatetime()+".png");
    if (!file.open(QIODevice::WriteOnly))
        gui->showTrayMessage(ClipperGUI::Error, ERROR_SAVING_SCREENSHOT.arg(file.errorString()));
    pixmap->save(&file, "PNG");
    file.close();
}

void ClipperCore::processHotkeyActivation(size_t activatedHotkey)
{
    switch (activatedHotkey)
    {
    case 1:
        processLinkShortening();
        break;
    case 2:
        processTextSharing();
        break;
    case 3:
        processFullScreenshot();
        break;
    case 4:
        processPartialScreenshot();
        break;
    case 5:
        processQRCode();
        break;
    case 6:
        gui->syncSettings();
        gui->show();
        break;
    };
}


void ClipperCore::putStringIntoClipboard(QString str)
{
    clipboard->setText(str);
}

void ClipperCore::updateSettings()
{
    for (QMetaObject::Connection connection : connections.values())
        disconnect(connection);

    qDebug() << settings->shortcuts["ShortenLink"];

    if (settings->general["LinkShortening"])
        hotkeys->registerHotkey(UKeySequence(settings->shortcuts["ShortenLink"]), 1);

    if (settings->general["PastePublishing"])
        hotkeys->registerHotkey(UKeySequence(settings->shortcuts["PastePublish"]), 2);

    if (settings->general["ScreenshotMaking"])
        hotkeys->registerHotkey(UKeySequence(settings->shortcuts["Screenshot"]), 3);

    if (settings->general["PartialScreenshotMaking"])
        hotkeys->registerHotkey(UKeySequence(settings->shortcuts["PartialScreenshot"]), 4);

    if (settings->general["QRCodeMaking"])
        hotkeys->registerHotkey(UKeySequence(settings->shortcuts["QRCode"]), 5);

    if (settings->general["EnableMainWindowShortcut"])
        hotkeys->registerHotkey(UKeySequence(settings->shortcuts["MainWindow"]), 6);

    if (settings->general["KeepHistory"])
    {
        if (historyFile != nullptr)
            delete historyFile;

        historyFile = new QFile(settings->historyFilePath);
        historyFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

        connections["historyupdate"] = connect(clipboard, &QClipboard::dataChanged, [=]() {
            QString text = this->getClipboardText();

            QTextStream historyFileStream(historyFile);
            historyFileStream << QString("----------------%1----------------\n %2 \n\n").arg(getCurrentDatetime(), text);
            gui->updateHistory(text);
        });
    }

    connections["hotkeys"] = connect(hotkeys, SIGNAL(activated(size_t)), this, SLOT(processHotkeyActivation(size_t)), Qt::UniqueConnection);
}
