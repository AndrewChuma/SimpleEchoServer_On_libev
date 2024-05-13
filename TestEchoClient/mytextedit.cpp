#include "mytextedit.h"

#include <QEvent>
#include <QDebug>

C_MyTextEdit::C_MyTextEdit(QWidget *a_parent) : QPlainTextEdit(a_parent)
{

}

void C_MyTextEdit::keyPressEvent(QKeyEvent *e)
{
    if ( e->modifiers() == Qt::ShiftModifier && e->key() == Qt::Key_Return  ) {

        QPlainTextEdit::keyPressEvent(e);
        return;

    }

    if ( e->key() == Qt::Key_Return ) {
        emit returnPressed();
        return;
    }

    QPlainTextEdit::keyPressEvent(e);
}
