#include "clipper.h"

Clipper::Clipper(QObject *parent) :
    QObject(parent)
{
    clipboard = QApplication::clipboard();
    QxtGlobalShortcut *shortcut = new QxtGlobalShortcut(QKeySequence("F6"));
    connect(shortcut, SIGNAL(activated()), this, SLOT(onShortcutActivated()));
}

Clipper::~Clipper()
{
}

void Clipper::googleLinkShorten(QString link)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkRequest request;
    request.setUrl(QUrl("https://www.googleapis.com/urlshortener/v1/url"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QVariantMap args;
    args["longUrl"] = link;

    manager->post(request, JSON::serialize(QVariant(args), 4));
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onLinkShortened(QNetworkReply*)));
}

void Clipper::onLinkShortened(QNetworkReply* reply)
{
    QVariantMap response = JSON::parse(reply->readAll()).toMap();
    clipboard->setText(response["id"].toString());
}

void Clipper::onShortcutActivated()
{
    googleLinkShorten(clipboard->text(QClipboard::Selection));
}
