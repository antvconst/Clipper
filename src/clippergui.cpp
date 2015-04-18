#include "clippergui.h"

ClipperGUI::ClipperGUI(ClipperCore *core, QWidget *parent) :
    QWidget(parent), ui(new Ui::Clipper), core(core)
{
    ui->setupUi(this);
    appIcon.addFile(":/misc/icon.ico");

    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = (screenGeometry.width()-this->width()) / 2;
    int y = (screenGeometry.height()-this->height()) / 2;
    this->move(x, y);

    this->setWindowIcon(appIcon);
    this->hide();

    hotkeyEditWindow = new QHotkeyEdit;

    QMenu *trayMenu = new QMenu();
    trayMenu->addAction("Quit", qApp, SLOT(quit()));
    tray = new QSystemTrayIcon();
    tray->setIcon(appIcon);
    tray->setContextMenu(trayMenu);
    tray->show();

    shortcutButtons.addButton(ui->shortenLinkButton, 1);
    shortcutButtons.addButton(ui->publishPasteButton, 2);
    shortcutButtons.addButton(ui->makeScreenshotButton, 3);
    shortcutButtons.addButton(ui->makeQRCodeButton, 4);
    shortcutButtons.addButton(ui->makePartialScreenshotButton, 5);
    shortcutButtons.addButton(ui->mainWindowButton, 6);

    settings = ClipperSettings::instance();

    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveSettings()));

    connect(ui->clearButton, &QPushButton::clicked, [=]() {
        ui->listWidget->clear();
    });

    connect(ui->listWidget, &QListWidget::itemClicked, [=](QListWidgetItem* listItem) {
        core->putStringIntoClipboard(listItem->text());
        this->showTrayMessage(ClipperGUI::Notification, "History item is copied into clipboard");
    });

    connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
            SLOT(processTrayIconClick(QSystemTrayIcon::ActivationReason)));

    // Strange construction in the next line is used to define which overloaded buttonClicked signal should be used (the int one)
    connect(&shortcutButtons, static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked), [=](int id) -> void {
        if (hotkeyEditWindow->isVisible())
            return;

        hotkeyEditWindow = new QHotkeyEdit();
        hotkeyEditWindow->show();
        hotkeyUnderChangeId = id;
        connect(hotkeyEditWindow, SIGNAL(hotkeyReady(QString)), this, SLOT(changeHotkey(QString)));
    });

    connect(ui->changeScreenshotPath, &QPushButton::clicked, [=]() {
        QString path = QFileDialog::getExistingDirectory(this,
                                                         tr("Open Directory"),
                                                         "",
                                                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        if (!path.isEmpty())
            ui->screenshotPathEdit->setText(path);
    });

    connect(ui->changeHistoryFilePath, &QPushButton::clicked, [=]() {
        QString path = QFileDialog::getSaveFileName(0, "History file", QDir::homePath());
        if (!path.isEmpty())
            ui->historyPathEdit->setText(path);
    });

    this->syncSettings();
}

ClipperGUI::~ClipperGUI()
{
}

void ClipperGUI::updateHistory(QString text)
{
    if (ui->listWidget->findItems(text, Qt::MatchExactly).isEmpty())
    {
        ui->listWidget->addItem(text);
        ui->listWidget->itemAt(ui->listWidget->count(), 0)->setToolTip(QTime::currentTime().toString());
    }
}


void ClipperGUI::changeHotkey(QString hotkey)
{
    if (!settings->shortcuts.values().contains(hotkey) || hotkey == shortcutButtons.button(hotkeyUnderChangeId)->text())
    {
        shortcutButtons.button(hotkeyUnderChangeId)->setText(hotkey);
        hotkeyUnderChangeId = 0;
    }
    else
        this->showTrayMessage(ClipperGUI::CriticalError, "Hotkey is already in use");
}

void ClipperGUI::saveSettings()
{
    settings->shortcuts["ShortenLink"] = ui->shortenLinkButton->text();
    settings->shortcuts["PastePublish"] = ui->publishPasteButton->text();
    settings->shortcuts["Screenshot"] = ui->makeScreenshotButton->text();
    settings->shortcuts["PartialScreenshot"] = ui->makePartialScreenshotButton->text();
    settings->shortcuts["QRCode"] = ui->makeQRCodeButton->text();
    settings->shortcuts["MainWindow"] = ui->mainWindowButton->text();

    settings->general["LinkShortening"] = ui->linkShortening->isChecked();
    settings->general["PastePublishing"] = ui->pastePublishing->isChecked();
    settings->general["ScreenshotMaking"] = ui->screenshotMaking->isChecked();
    settings->general["PartialScreenshotMaking"] = ui->partialScreenshotMaking->isChecked();
    settings->general["QRCodeMaking"] = ui->QRCode->isChecked();
    settings->general["KeepHistory"] = ui->keepHistory->isChecked();
    settings->general["SaveScreenshots"] = ui->saveScreenshots->isChecked();
    settings->general["EnableMainWindowShortcut"] = ui->mainWindowShortcutCheckBox->isChecked();
    settings->screenshotPath = ui->screenshotPathEdit->text();
    settings->historyFilePath = ui->historyPathEdit->text();

    settings->saveSettings();
}

void ClipperGUI::processTrayIconClick(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::DoubleClick)
    {
        if (this->isVisible())
            this->hide();
        else
        {
            this->show();
            this->syncSettings();
        }
    }
}

void ClipperGUI::syncSettings()
{
    ui->shortenLinkButton->setText(settings->shortcuts["ShortenLink"]);
    ui->publishPasteButton->setText(settings->shortcuts["PastePublish"]);
    ui->makeScreenshotButton->setText(settings->shortcuts["Screenshot"]);
    ui->makePartialScreenshotButton->setText(settings->shortcuts["PartialScreenshot"]);
    ui->makeQRCodeButton->setText(settings->shortcuts["QRCode"]);
    ui->mainWindowButton->setText(settings->shortcuts["MainWindow"]);

    ui->linkShortening->setChecked(settings->general["LinkShortening"]);
    ui->pastePublishing->setChecked(settings->general["PastePublishing"]);
    ui->screenshotMaking->setChecked(settings->general["ScreenshotMaking"]);
    ui->partialScreenshotMaking->setChecked(settings->general["PartialScreenshotMaking"]);
    ui->QRCode->setChecked(settings->general["QRCodeMaking"]);
    ui->keepHistory->setChecked(settings->general["KeepHistory"]);
    ui->saveScreenshots->setChecked(settings->general["SaveScreenshots"]);
    ui->mainWindowShortcutCheckBox->setChecked(settings->general["EnableMainWindowShortcut"]);
    ui->screenshotPathEdit->setText(settings->screenshotPath);
    ui->historyPathEdit->setText(settings->historyFilePath);
}

void ClipperGUI::showTrayMessage(ClipperGUI::MessageType messageType, QString message)
{
    switch (messageType)
    {
    case Error:
        tray->showMessage("", message, QSystemTrayIcon::Warning, 3000);
        break;
    case Notification:
        tray->showMessage("", message, QSystemTrayIcon::Information, 3000);
        break;
    case CriticalError:
        tray->showMessage("", message, QSystemTrayIcon::Critical, 3000);
        break;
    };
}

void ClipperGUI::displayQrCode(std::shared_ptr<QPixmap> qrCode)
{
    QLabel *qrCodeWindow = new QLabel();
    qrCodeWindow->setPixmap(*qrCode);
    qrCodeWindow->setAlignment(Qt::AlignCenter);
    qrCodeWindow->setWindowIcon(appIcon);
    qrCodeWindow->setWindowFlags(Qt::WindowCloseButtonHint);
    qrCodeWindow->setAttribute(Qt::WA_DeleteOnClose);
    qrCodeWindow->show();
}

bool ClipperGUI::cropImage(std::shared_ptr<QPixmap> image)
{
    imageCrop = new ImageCropWidget;
    imageCrop->setImage(image);

    if (!imageCrop->proceed())
        return false;
    else
    {
        *image = imageCrop->getCroppedImage();
        delete imageCrop;
        return true;
    }
}

void ClipperGUI::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    hotkeyEditWindow->close();
}
