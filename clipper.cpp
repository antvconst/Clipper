#include "clipper.h"

Clipper::Clipper(QObject *parent) :
    QObject(parent)
{
    clipboard = QApplication::clipboard();
    pastebinPublish("test paste");
    //QxtGlobalShortcut *shortcut = new QxtGlobalShortcut(QKeySequence("F6"));
    //connect(shortcut, SIGNAL(activated()), this, SLOT(onShortcutActivated()));
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

void Clipper::pastebinPublish(QString text)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkRequest request;

    request.setUrl(QUrl("http://pastebin.com/api/api_post.php"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "text/plain");
    QByteArray requestData("api_option=paste&api_dev_key=a7bf92a1483fcce3f01f4b629bc9a9&api_paste_code="+QUrl::toPercentEncoding(text));
    qDebug() << requestData;
    manager->post(request, requestData);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onPasteLinkReady(QNetworkReply*)));
}

void Clipper::onPasteLinkReady(QNetworkReply *reply)
{
    qDebug() << reply->readAll();
}
