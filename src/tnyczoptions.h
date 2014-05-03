#ifndef TNYCZOPTIONS_H
#define TNYCZOPTIONS_H

#include <QWidget>
#include <QCloseEvent>
#include <QShowEvent>
#include <QDesktopWidget>

namespace Ui {
class TnyczOptions;
}

class TnyczOptions : public QWidget
{
    Q_OBJECT
    
public:
    explicit TnyczOptions(QWidget *parent = 0);
    ~TnyczOptions();
    void setPasteText(QString text);

    struct TextToPublish
    {
        bool isCode;
        bool isPrivate;
        bool isProtected;
        QString text;
        QString title;
        QString password;
    };

signals:
    void optionsReady(TnyczOptions::TextToPublish);
    void opened(bool);

private slots:
    void isProtectedCheckboxToggled(bool checked);
    void returnPublishOptions();

    void isCodeCheckboxToggled(bool checked);

    void isPrivateCheckboxToggled(bool checked);

private:
    Ui::TnyczOptions *ui;
    bool isCode = false;
    bool isPrivate = false;
    bool isProtected = false;
    QString pasteText;

protected:
    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *);
};

#endif // TNYCZOPTIONS_H
