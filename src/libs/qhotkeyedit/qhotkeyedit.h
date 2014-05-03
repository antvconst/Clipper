#ifndef HOTKEYEDIT_H
#define HOTKEYEDIT_H

#include <QLineEdit>
#include <QKeyEvent>
#include <QDebug>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QRegExpValidator>

#include "ui_qkeyedit.h"

namespace Ui
{
class QHotkeyEdit;
}

class QHotkeyEdit : public QWidget
{
    Q_OBJECT
public:
    explicit QHotkeyEdit(QWidget *parent = 0);
    
signals:
    void hotkeyReady(QString);
    void opened(bool);

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *event);

private slots:
    void on_pushButton_clicked();

private:
    Ui::QHotkeyEdit *ui;
    QString textHotkey;

};

#endif // HOTKEYEDIT_H
