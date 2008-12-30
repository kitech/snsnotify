#ifndef MESSAGE_HINT_H
#define MESSAGE_HINT_H

#include <QtCore>
#include <QtGui>

#include "ui_messagehint.h"

class MessageHint : public QWidget
{
    Q_OBJECT;
public:
    MessageHint(QWidget *parent = 0);
    ~MessageHint();

public slots:
    void show(QString msg);
private slots:
    void dynShowTimeout();
    void hideDidaTimeout();

private:
    Ui::MessageHint  ui_win;
    QString msgTpl;
    QTimer timer;
    QTimer hideDida;
    QBitmap img;
};

#endif

