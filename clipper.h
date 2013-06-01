#ifndef CLIPPER_H
#define CLIPPER_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QList>
#include <QClipboard>
#include <QKeySequence>
#include <QApplication>
#include "json/json.h"
#include "qxt/qxtglobalshortcut.h"

#include <QDebug>

namespace Ui {
class Clipper;
}

class Clipper : public QObject
{
    Q_OBJECT
    
public:
    explicit Clipper(QObject *parent = 0);
    ~Clipper();
    void googleLinkShorten(QString link);

private slots:
    void onLinkShortened(QNetworkReply* reply);
    void onShortcutActivated();

private:
    Ui::Clipper *ui;
    QClipboard *clipboard;
};

#endif // CLIPPER_H
