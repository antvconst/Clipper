#include "clipperapis.h"

ClipperAPIs::ClipperAPIs(QObject *parent) :
    QObject(parent)
{
}

void ClipperAPIs::googleLinkShorten(QString link)
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

void ClipperAPIs::onLinkShortened(QNetworkReply* reply)
{
    QVariantMap response = JSON::parse(reply->readAll()).toMap();
    emit linkReady((response["id"].toString()));
}

void ClipperAPIs::tnyczPublish
(QString text, QString title, QString password, bool is_code, bool is_private, bool is_protected)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkRequest request(QUrl("http://tny.cz/api/create.json"));

    QByteArray data;
    data += "paste="+QUrl::toPercentEncoding(text);
    data += "&title="+QUrl::toPercentEncoding(title);
    if (is_code)
        data += "&is_code=1";
    if (is_private)
        data += "&is_private=1";
    if (is_protected)
        data += "&password="+QUrl::toPercentEncoding(password);
    manager->post(request, data);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onPasteLinkReady(QNetworkReply*)));
}

void ClipperAPIs::onPasteLinkReady(QNetworkReply *reply)
{
    emit linkReady("http://tny.cz/"+JSON::parse(
                       reply->readAll()).toMap()["result"].toMap()["response"].toString());
}

void ClipperAPIs::imageshackUpload(QByteArray &picture)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkRequest request(QUrl("https://post.imageshack.us/upload_api.php"));
    request.setRawHeader("Host", "imageshack.us");
    request.setRawHeader("Content-type", "multipart/form-data, boundary=AyV04a");
    request.setRawHeader("Cache-Control", "no-cache");
    request.setRawHeader("Accept","*/*");

    QByteArray requestData;

    requestData.append("--AyV04a\r\n");
    requestData.append("content-disposition: ");
    requestData.append("form-data; name=\"key\"\r\n");
    requestData.append("\r\n");
    requestData.append("46GINSVY90936a01bc287a245cccc30026678653");
    requestData.append("\r\n");

    requestData.append("--AyV04a\r\n");
    requestData.append("content-type: ");
    requestData.append("image/png\r\n");
    requestData.append("content-disposition: ");
    requestData.append(QString("file; name=\"fileupload\"; filename=\"%1.png\"\r\n").arg(QTime::currentTime().toString()));
    requestData.append("Content-Transfer-Encoding: binary\r\n");
    requestData.append("\r\n");
    requestData.append(picture);
    requestData.append("\r\n");

    requestData.append("--AyV04a\r\n");
    requestData.append("content-disposition: ");
    requestData.append("form-data; name=\"format\"\r\n");
    requestData.append("\r\n");
    requestData.append("json\r\n");
    requestData.append("--AyV04a--");

    manager->post(request, requestData);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onImageshackUploaded(QNetworkReply*)));
}

void ClipperAPIs::onImageshackUploaded(QNetworkReply *response)
{
    QVariantMap responseMap = JSON::parse(response->readAll()).toMap();
    QVariantMap linkMap = responseMap["links"].toMap();
    emit linkReady(linkMap["image_link"].toString());
}
