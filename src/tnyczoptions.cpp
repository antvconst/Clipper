#include "tnyczoptions.h"
#include "ui_tnyczoptions.h"

TnyczOptions::TnyczOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TnyczOptions)
{
    ui->setupUi(this);

    is_code = true;
    is_private = true;

    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = (screenGeometry.width()-this->width()) / 2;
    int y = (screenGeometry.height()-this->height()) / 2;
    this->move(x, y);

    setWindowIcon(QIcon(":/icon.ico"));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->publishButton, SIGNAL(clicked()), this, SLOT(returnPublishOptions()));

    ui->cancelButton->setShortcut(QKeySequence("Esc"));
    ui->publishButton->setShortcut(QKeySequence("Enter"));
}

TnyczOptions::~TnyczOptions()
{
    delete ui;
}

void TnyczOptions::setPasteText(QString text)
{
    pasteText = text;
}


void TnyczOptions::on_isProtectedCheckbox_toggled(bool checked)
{
    ui->passwordEdit->setEnabled(checked);
    is_protected = checked;
}

void TnyczOptions::on_isCodeCheckbox_toggled(bool checked)
{
    is_code = checked;
}

void TnyczOptions::on_isPrivateCheckbox_toggled(bool checked)
{
    is_private = checked;
}

void TnyczOptions::closeEvent(QCloseEvent *event)
{
    emit opened(false);
}

void TnyczOptions::showEvent(QShowEvent *)
{
    emit opened(true);
}

void TnyczOptions::returnPublishOptions()
{
    TextToPublish textToPublish;

    textToPublish.is_code = is_code;
    textToPublish.is_private = is_private;
    textToPublish.is_protected = is_protected;
    textToPublish.text = pasteText;
    textToPublish.password = ui->passwordEdit->text();
    textToPublish.title = ui->pasteNameEdit->text();

    emit optionsReady(textToPublish);
    this->close();
}
