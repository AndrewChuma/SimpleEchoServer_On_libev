#ifndef MYTEXTEDIT_H
#define MYTEXTEDIT_H

#include <QPlainTextEdit>

class C_MyTextEdit : public QPlainTextEdit
{

    Q_OBJECT

public:

    C_MyTextEdit(QWidget *a_parent = nullptr);

protected:

    void keyPressEvent(QKeyEvent *e) override;

signals:

    void returnPressed();

};

#endif // MYTEXTEDIT_H
