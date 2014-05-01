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
        bool is_code;
        bool is_private;
        bool is_protected;
        QString text;
        QString title;
        QString password;
    };

signals:
    void optionsReady(TnyczOptions::TextToPublish);
    void opened(bool);

private slots:
    void on_isProtectedCheckbox_toggled(bool checked);
    void returnPublishOptions();

    void on_isCodeCheckbox_toggled(bool checked);

    void on_isPrivateCheckbox_toggled(bool checked);

private:
    Ui::TnyczOptions *ui;
    bool is_code = false;
    bool is_private = false;
    bool is_protected = false;
    QString pasteText;

protected:
    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *);
};

#endif // TNYCZOPTIONS_H
