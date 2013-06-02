#include "tnyczoptions.h"
#include "ui_tnyczoptions.h"

TnyczOptions::TnyczOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TnyczOptions)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/three.ico"));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->publishButton, SIGNAL(clicked()), this, SLOT(returnPublishOptions()));
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

void TnyczOptions::returnPublishOptions()
{
    emit optionsReady(pasteText, ui->pasteNameEdit->text(), ui->passwordEdit->text(), is_code, is_private, is_protected);
    this->close();
}
