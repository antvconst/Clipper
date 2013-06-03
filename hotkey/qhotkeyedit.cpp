#include "qhotkeyedit.h"

QHotkeyEdit::QHotkeyEdit(QWidget *parent) :
    QWidget(parent), ui(new Ui::QHotkeyEdit)
{
    ui->setupUi(this);
    this->installEventFilter(this);
}

bool QHotkeyEdit::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int keyInt = keyEvent->key();
        Qt::Key key = static_cast<Qt::Key>(keyInt);

        if (key == Qt::Key_Escape) this->close();
        else
        if (key != Qt::Key_Control && key != Qt::Key_Shift && key!= Qt::Key_Backspace &&
                key != Qt::Key_Alt && key != Qt::Key_Meta && key != Qt::Key_unknown)
        {
            Qt::KeyboardModifiers modifiers = keyEvent->modifiers();

            if(modifiers & Qt::ShiftModifier)
                keyInt += Qt::SHIFT;
            if(modifiers & Qt::ControlModifier)
                keyInt += Qt::CTRL;
            if(modifiers & Qt::AltModifier)
                keyInt += Qt::ALT;
            if(modifiers & Qt::MetaModifier)
                keyInt += Qt::META;

            textHotkey = QKeySequence(keyInt).toString(QKeySequence::PortableText);
            ui->lineEdit->setText(textHotkey);
        }
    }
}
void QHotkeyEdit::on_pushButton_clicked()
{
    emit hotkeyReady(textHotkey);
    this->close();
}
