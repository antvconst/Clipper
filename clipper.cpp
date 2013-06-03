#include "clipper.h"

Clipper::Clipper(QWidget *parent) :
    QWidget(parent), ui(new Ui::Clipper)
{
    ui->setupUi(this);

    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = (screenGeometry.width()-this->width()) / 2;
    int y = (screenGeometry.height()-this->height()) / 2;
    this->move(x, y);

    this->hide();

    settings = new QSettings();

    clipboard = QApplication::clipboard();
    api = new ClipperAPIs();

    QMenu *trayMenu = new QMenu();
    trayMenu->addAction("Quit", qApp, SLOT(quit()));

    tray = new QSystemTrayIcon(this);
    tray->setIcon(QIcon(":/clipper.ico"));
    tray->setContextMenu(trayMenu);
    tray->show();

    shortcutButtons.addButton(ui->shortenLinkButton, 1);
    shortcutButtons.addButton(ui->publishPasteButton, 2);
    shortcutButtons.addButton(ui->makeScreenshotButton, 3);

    shortcuts["ShortenLink"] = settings->value("Hotkeys/ShortenLink", "F6").toString();
    shortcuts["PastePublish"] = settings->value("Hotkeys/PastePublish", "F7").toString();
    shortcuts["Screenshot"] = settings->value("Hotkeys/Screenshot", "F8").toString();

    initHotkeys();

    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveSettings()));
    connect(ui->clearButton, SIGNAL(clicked()), ui->listWidget, SLOT(clear()));
    connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
            SLOT(onTrayIconClicked(QSystemTrayIcon::ActivationReason)));
    connect(clipboard, SIGNAL(dataChanged()), this, SLOT(updateHistory()));
    connect(linkShortenShortcut, SIGNAL(activated()), this, SLOT(onLinkShortenShortcutActivated()));
    connect(tnyczPublishShortcut, SIGNAL(activated()), this, SLOT(onTnyczPublishShortcutActivated()));
    connect(api, SIGNAL(linkReady(QString)), this, SLOT(linkToClipboard(QString)));
    connect(&shortcutButtons, SIGNAL(buttonClicked(int)), this, SLOT(onChangeHotkeyButtonClicked(int)));
    connect(screenshotShortcut, SIGNAL(activated()), this, SLOT(makeScreenshot()));
}

Clipper::~Clipper()
{
}

void Clipper::onLinkShortenShortcutActivated()
{
    if (!clipboard->text().isEmpty())
        api->googleLinkShorten(clipboard->text(QClipboard::Selection));
    else
        tray->showMessage("", "Selection is empty");
}

void Clipper::onTnyczPublishShortcutActivated()
{
    if (!clipboard->text().isEmpty())
    {
        TnyczOptions *tnycz = new TnyczOptions();
        tnycz->show();
        tnycz->setPasteText(clipboard->text(QClipboard::Selection));
        connect(tnycz, SIGNAL(optionsReady(QString,QString,QString,bool,bool,bool)), api,
                SLOT(tnyczPublish(QString,QString,QString,bool,bool,bool)));
    }
    else
        tray->showMessage("", "Selection is empty");
}

void Clipper::linkToClipboard(QString link)
{
    clipboard->setText(link);
    tray->showMessage("", "Link is copied to your clipboard.");
}

void Clipper::updateHistory()
{
    ui->listWidget->addItem(QTime::currentTime().toString()+". "+clipboard->text());
}

void Clipper::onChangeHotkeyButtonClicked(int id)
{
    QHotkeyEdit *window = new QHotkeyEdit();
    window->show();
    button_id = id;
    connect(window, SIGNAL(hotkeyReady(QString)), this, SLOT(changeHotkey(QString)));
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

}

void Clipper::onTrayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::DoubleClick)
        this->show();
}

void Clipper::makeScreenshot()
{
    QPixmap screenshot = QPixmap::grabWindow(QApplication::desktop()->winId());
    QByteArray screenshotData;
    QBuffer buffer(&screenshotData);
    buffer.open(QIODevice::WriteOnly);
    screenshot.save(&buffer, "PNG");
    tray->showMessage("", "Uploading screenshot, please wait..");
    api->imageshackUpload(screenshotData);
}

void Clipper::initHotkeys()
{
    linkShortenShortcut = new QxtGlobalShortcut(QKeySequence(shortcuts["ShortenLink"]));
    tnyczPublishShortcut = new QxtGlobalShortcut(QKeySequence(shortcuts["PastePublish"]));
    screenshotShortcut = new QxtGlobalShortcut(QKeySequence(shortcuts["Screenshot"]));

    ui->shortenLinkButton->setText(shortcuts["ShortenLink"]);
    ui->publishPasteButton->setText(shortcuts["PastePublish"]);
    ui->makeScreenshotButton->setText(shortcuts["Screenshot"]);
}
