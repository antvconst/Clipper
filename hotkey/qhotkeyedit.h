#ifndef HOTKEYEDIT_H
#define HOTKEYEDIT_H

#include <QLineEdit>
#include <QKeyEvent>
#include <QDebug>
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

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void on_pushButton_clicked();

private:
    Ui::QHotkeyEdit *ui;
    QString textHotkey;

};

#endif // HOTKEYEDIT_H
