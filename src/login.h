#ifndef LOGIN_H
#define LOGIN_H

#include <QtGui>

#include "ui_login.h"

class Login : public QWidget
{
    Q_OBJECT;
public:
    Login(QWidget *parent = 0);
    ~Login();

public slots:
    void showLoginMessage(QString msg);
    void loginFaild(int status, QString reason);

private slots:
    void login();
    void showStatus(QString str);
    void onUserNameSelectChanged(int);

signals:
    void wantLogin(QString, QString, QList<int>);
protected:
    void showEvent ( QShowEvent * event );
private:
    Ui::LoginWindow ui_win;
    char sessLoaded;
};

#endif
