#include "clipper.h"

Clipper::Clipper(QWidget *parent) :
    QWidget(parent)
{
    this->hide();
    clipboard = QApplication::clipboard();
    api = new ClipperAPIs();
    tray = new QSystemTrayIcon(this);
    tray->setIcon(QIcon(":/clipper.ico"));
    tray->show();
    QxtGlobalShortcut *linkShortenShortcut = new QxtGlobalShortcut(QKeySequence("F6"));
    QxtGlobalShortcut *tnyczPublishShortcut = new QxtGlobalShortcut(QKeySequence("F7"));

    connect(linkShortenShortcut, SIGNAL(activated()), this, SLOT(onLinkShortenShortcutActivated()));
    connect(tnyczPublishShortcut, SIGNAL(activated()), this, SLOT(onTnyczPublishShortcutActivated()));
    connect(api, SIGNAL(linkReady(QString)), this, SLOT(linkToClipboard(QString)));
}

Clipper::~Clipper()
{
}

void Clipper::onLinkShortenShortcutActivated()
{
    api->googleLinkShorten(clipboard->text(QClipboard::Selection));
}

void Clipper::onTnyczPublishShortcutActivated()
{
    TnyczOptions *tnycz = new TnyczOptions();
    tnycz->show();
    tnycz->setPasteText(clipboard->text(QClipboard::Selection));
    connect(tnycz, SIGNAL(optionsReady(QString,QString,QString,bool,bool,bool)), api,
            SLOT(tnyczPublish(QString,QString,QString,bool,bool,bool)));
}

void Clipper::linkToClipboard(QString link)
{
    clipboard->setText(link);
    tray->showMessage("", "Link is copied to your clipboard.");
}

// 1039
