#ifndef TNYCZOPTIONS_H
#define TNYCZOPTIONS_H

#include <QWidget>

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

signals:
    void optionsReady(QString, QString, QString, bool, bool, bool);

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
};

#endif // TNYCZOPTIONS_H
