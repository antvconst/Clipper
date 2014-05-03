#include "tnyczoptions.h"
#include "ui_tnyczoptions.h"

TnyczOptions::TnyczOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TnyczOptions)
{
    ui->setupUi(this);

    isCode = true;
    isPrivate = true;

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


void TnyczOptions::isProtectedCheckboxToggled(bool checked)
{
    ui->passwordEdit->setEnabled(checked);
    isProtected = checked;
}

void TnyczOptions::isCodeCheckboxToggled(bool checked)
{
    isCode = checked;
}

void TnyczOptions::isPrivateCheckboxToggled(bool checked)
{
    isPrivate = checked;
}

void TnyczOptions::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    emit opened(false);
}

void TnyczOptions::showEvent(QShowEvent *)
{
    emit opened(true);
}

void TnyczOptions::returnPublishOptions()
{
    TextToPublish textToPublish;

    textToPublish.isCode = isCode;
    textToPublish.isPrivate = isPrivate;
    textToPublish.isProtected = isProtected;
    textToPublish.text = pasteText;
    textToPublish.password = ui->passwordEdit->text();
    textToPublish.title = ui->pasteNameEdit->text();

    emit optionsReady(textToPublish);
    this->close();
}
