#ifndef CHAT_WINDOW_H
#define CHAT_WINDOW_H

#include <QtCore>
#include <QtGui>

#include "ui_chatwindow.h"

class ChatWindow : public QMainWindow
{
    Q_OBJECT;
public:
    ChatWindow(QString uid, QString username, QWidget *parent = 0);
    ~ChatWindow();

signals:
    void sendMsg(QString uid, QString msg);

private slots:
    void sendMsg();

private:
    Ui::ChatWindow ui_win;
    QString uid;
    QString username;
    QWidget *center_win;
};


#endif
