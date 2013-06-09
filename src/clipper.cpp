#include "clipper.h"

Clipper::Clipper(QWidget *parent) :
    QWidget(parent), ui(new Ui::Clipper)
{
    ui->setupUi(this);
    appIcon.addFile(":/clipper.ico");

    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = (screenGeometry.width()-this->width()) / 2;
    int y = (screenGeometry.height()-this->height()) / 2;
    this->move(x, y);
    this->setWindowIcon(appIcon);

    this->hide();

    QString settingsFilePath = QDir::homePath()+"/.clipper";
    settings = new QSettings(settingsFilePath, QSettings::NativeFormat);

    clipboard = QApplication::clipboard();
    api = new ClipperAPIs();
    tnyczOptionsWindow = new TnyczOptions;
    hotkeyEditWindow = new QHotkeyEdit;

    QMenu *trayMenu = new QMenu();
    trayMenu->addAction("Quit", qApp, SLOT(quit()));

    tray = new QSystemTrayIcon(this);

    tray->setIcon(appIcon);
    tray->setContextMenu(trayMenu);
    tray->show();

    shortcutButtons.addButton(ui->shortenLinkButton, 1);
    shortcutButtons.addButton(ui->publishPasteButton, 2);
    shortcutButtons.addButton(ui->makeScreenshotButton, 3);

    initHotkeys();

    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveSettings()));
    connect(ui->clearButton, &QPushButton::clicked, [=]() { ui->listWidget->clear(); });
    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(historyItemToClipboard(QListWidgetItem*)));
    connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
            SLOT(onTrayIconClicked(QSystemTrayIcon::ActivationReason)));
    connect(clipboard, SIGNAL(dataChanged()), this, SLOT(updateHistory()));
    connect(api, SIGNAL(linkReady(QString)), this, SLOT(linkToClipboard(QString)));
    connect(&shortcutButtons, SIGNAL(buttonClicked(int)), this, SLOT(onChangeHotkeyButtonClicked(int)));
}

Clipper::~Clipper()
{
    disconnect(this);
    linkShortenShortcut->disconnect(this);
    tnyczPublishShortcut->disconnect(this);
    screenshotShortcut->disconnect(this);
}

void Clipper::linkShorten()
{
#ifdef Q_OS_LINUX
    if (!clipboard->text(QClipboard::Selection).isEmpty())
        api->googleLinkShorten(clipboard->text(QClipboard::Selection));
#elif Q_OS_WIN
    if (!clipboard->text().isEmpty())
        api->googleLinkShorten(clipboard->text());
#endif
    else
        tray->showMessage("", "Selection is empty", QSystemTrayIcon::Information, 3000);
}

void Clipper::tnyczPublish()
{
    if (tnyczOptionsWindow->isVisible())
        return;

#ifdef Q_OS_LINUX
    if (!clipboard->text(QClipboard::Selection).isEmpty())
#elif Q_OS_WIN
    if (!clipboard->text().isEmpty())
#endif
    {
        tnyczOptionsWindow = new TnyczOptions();
        tnyczOptionsWindow->show();
#ifdef Q_OS_LINUX
        tnyczOptionsWindow->setPasteText(clipboard->text(QClipboard::Selection));
#elif Q_OS_WIN
        tnycz->setPasteText(clipboard->text());
#endif
        connect(tnyczOptionsWindow, SIGNAL(optionsReady(QString,QString,QString,bool,bool,bool)), api,
                SLOT(tnyczPublish(QString,QString,QString,bool,bool,bool)));
    }
    else
        tray->showMessage("", "Selection is empty", QSystemTrayIcon::Information, 3000);
}

void Clipper::linkToClipboard(QString link)
{
    clipboard->setText(link);
    tray->showMessage("", "Link is copied to your clipboard.", QSystemTrayIcon::Information, 3000);
}

void Clipper::updateHistory()
{
    QString newText = clipboard->text();
    if (ui->listWidget->findItems(newText, Qt::MatchExactly).isEmpty())
    {
        ui->listWidget->addItem(newText);
        ui->listWidget->itemAt(ui->listWidget->count(), 0)->setToolTip(QTime::currentTime().toString());
    }
}

void Clipper::onChangeHotkeyButtonClicked(int id)
{
    if (hotkeyEditWindow->isVisible())
        return;

    hotkeyEditWindow = new QHotkeyEdit();
    hotkeyEditWindow->show();
    button_id = id;
    connect(hotkeyEditWindow, SIGNAL(hotkeyReady(QString)), this, SLOT(changeHotkey(QString)));
}

void Clipper::changeHotkey(QString hotkey)
{
    if (!shortcuts.values().contains(hotkey))
    {
        shortcutButtons.button(button_id)->setText(hotkey);
        button_id = 0;
    }
    else
        QMessageBox::critical(this, "", "This hotkey is already in use. Try another.",
                              QMessageBox::Ok);
}

void Clipper::saveSettings()
{
    settings->beginGroup("Hotkeys");
    settings->setValue("ShortenLink", ui->shortenLinkButton->text());
    settings->setValue("PastePublish", ui->publishPasteButton->text());
    settings->setValue("Screenshot", ui->makeScreenshotButton->text());
    settings->endGroup();
    initHotkeys();
}

void Clipper::onTrayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::DoubleClick)
    {
        show();
        ui->shortenLinkButton->setText(settings->value("Hotkeys/ShortenLink", "F6").toString());
        ui->publishPasteButton->setText(settings->value("Hotkeys/PastePublish", "F7").toString());
        ui->makeScreenshotButton->setText(settings->value("Hotkeys/Screenshot", "F8").toString());
    }
}

void Clipper::makeScreenshot()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QPixmap screenshot = screen->grabWindow(0);
    QByteArray screenshotData;
    QBuffer buffer(&screenshotData);
    buffer.open(QIODevice::WriteOnly);
    screenshot.save(&buffer, "PNG");
    tray->showMessage("", "Uploading screenshot, please wait..", QSystemTrayIcon::Information, 1000);
    api->imageshackUpload(screenshotData);
}

void Clipper::initHotkeys()
{
    if (!hotkeysInit)
    {
        linkShortenShortcut->disconnect(this);
        tnyczPublishShortcut->disconnect(this);
        screenshotShortcut->disconnect(this);

        delete linkShortenShortcut;
        delete tnyczPublishShortcut;
        delete screenshotShortcut;
        tray->showMessage("", "Hotkeys updated", QSystemTrayIcon::Information, 2000);
    }
    else
        hotkeysInit = false;

    shortcuts["ShortenLink"] = settings->value("Hotkeys/ShortenLink", "F6").toString();
    shortcuts["PastePublish"] = settings->value("Hotkeys/PastePublish", "F7").toString();
    shortcuts["Screenshot"] = settings->value("Hotkeys/Screenshot", "F8").toString();

    linkShortenShortcut = new QxtGlobalShortcut(QKeySequence(shortcuts["ShortenLink"]));
    tnyczPublishShortcut = new QxtGlobalShortcut(QKeySequence(shortcuts["PastePublish"]));
    screenshotShortcut = new QxtGlobalShortcut(QKeySequence(shortcuts["Screenshot"]));

    ui->shortenLinkButton->setText(shortcuts["ShortenLink"]);
    ui->publishPasteButton->setText(shortcuts["PastePublish"]);
    ui->makeScreenshotButton->setText(shortcuts["Screenshot"]);

    connect(linkShortenShortcut, SIGNAL(activated()), this, SLOT(linkShorten()));
    connect(tnyczPublishShortcut, SIGNAL(activated()), this, SLOT(tnyczPublish()));
    connect(screenshotShortcut, SIGNAL(activated()), this, SLOT(makeScreenshot()));
}

void Clipper::historyItemToClipboard(QListWidgetItem *item)
{
    clipboard->setText(item->text());
    tray->showMessage("", "History item is copied into clipboard.", QSystemTrayIcon::Information, 3000);
}

void Clipper::closeEvent(QCloseEvent *event)
{
    hotkeyEditWindow->close();
}
