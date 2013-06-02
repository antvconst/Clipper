#ifndef CLIPPER_H
#define CLIPPER_H

#include <QWidget>
#include <QClipboard>
#include <QKeySequence>
#include <QSystemTrayIcon>
#include <QApplication>
#include <QDebug>
#include "ui_clipper.h"

#include "clipperapis.h"
#include "tnyczoptions.h"
#include "qxt/qxtglobalshortcut.h"

namespace Ui {
class Clipper;
}

class Clipper : public QWidget
{
    Q_OBJECT
    
public:
    explicit Clipper(QWidget *parent = 0);
    ~Clipper();

private slots:
    void onLinkShortenShortcutActivated();
    void linkToClipboard(QString link);
    void onTnyczPublishShortcutActivated();

private:
    Ui::Clipper *ui;
    QSystemTrayIcon *tray;
    QClipboard *clipboard;
    ClipperAPIs *api;

};

#endif // CLIPPER_H
