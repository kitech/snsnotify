#ifndef SNS_NOTIFY_H
#define SNS_NOTIFY_H

#include <QtCore>
#include <QtGui>
#include <QtNetwork>

#include "ui_mainwindow.h"

class Account;
class FeedGearbox;
class LetterGearbox;
class MessageHint;
class LiveUpdate;
class Login;
class AccountPanel;

class SNSNotify : public QMainWindow
{
    Q_OBJECT;
public:
    SNSNotify(QWidget *parent = 0);
    ~SNSNotify();

    int get_version();

private:
    void setupSystemTray();
    void showTrayMessage(int count);

private slots:
    void cleanupMainWindow();
    void about();
    void systemTrayActivated(QSystemTrayIcon::ActivationReason reason);
    void systemTrayMessageClicked();
    void wantQuit();
    void adjustMainWindowSize();

    void loginStart(Account *acc);
    void loginDone(int status, Account *acc);

    void fetchFeedDone(int status, Account *acc);
    void setNewFeedNotice(bool note);

    void showMessageHint(QString title, QString msg);
    void showStatus(QString str);

    /* void initKWCompleter(); */
    /* void kwAddWord(QString word); */
    /* void kwClearWord(); */

    void test_live_update();

signals:
    void wantLogin(QString userName, QString password, QList<int> siteIds);
    void fetchFeed(Account *acc);

private:
    Ui::MainWindow ui_win;
    Login *login_win;
    AccountPanel *acc_panel;
    QSystemTrayIcon *sti;
    bool sizeAdjusted;
    QStatusBar *statBar;
    /* QString lastKeyword; */
    bool newFeedNotice;
    MessageHint *msgHint;
    /* QCompleter *kwCompleter; */
    LiveUpdate *updater;

};

#endif

