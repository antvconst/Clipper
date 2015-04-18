#ifndef CLIPPERNETWORK_H
#define CLIPPERNETWORK_H

#include <memory>

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QVariantMap>
#include <QPixmap>
#include <QUrl>
#include <QHttpMultiPart>
#include <QUrlQuery>
#include <QDebug>

#include "libs/json/json.h"

class ClipperNetwork : public QObject
{
    Q_OBJECT

public:
    static ClipperNetwork* instance()
    {
        static ClipperNetwork *instance = new ClipperNetwork();
        return instance;
    }

public slots:
    void uploadImage(std::shared_ptr<QByteArray> image);
    void shortenLink(QString link);
    void convertToQRCode(QString text);
    void shareText(QString& text);

private:
    QNetworkAccessManager *network;

    ClipperNetwork(QObject *parent = 0);
    ClipperNetwork(const ClipperNetwork& root);
    ClipperNetwork& operator=(const ClipperNetwork&);

    const QString IMAGE_UPLOAD_URL = "http://deviantsart.com/";
    const QString HASTEBIN_BASE_URL = "http://hastebin.com/";
    const QString HASTEBIN_PUBLISH_URL = "http://hastebin.com/documents";
    const QString LINK_SHORTEN_URL = "https://www.googleapis.com/urlshortener/v1/url?key=AIzaSyDLjDEjx-58W3BTgtopwpqH5cLjd3TgLvI";
    const QString QR_CODE_URL = "http://qrickit.com/api/qr";
    const QString UNKNOWN_ERROR = "Unknown error occured";

signals:
    void linkReady(QString link);
    void qrCodeReady(std::shared_ptr<QPixmap> qrCode);
    void failure(QString errorString);
};

#endif // CLIPPERNETWORK_H
