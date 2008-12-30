#ifndef ACCOUNT_PANEL_H
#define ACCOUNT_PANEL_H

#include <QtCore>
#include <QtGui>

#include "ui_accountpanel.h"

class Account;
//class SearchBar;
class FeedGearbox;
class FriendItemDelegate;
class ChatWindow;

class AccountPanel : public QWidget
{
    Q_OBJECT;
public:
    AccountPanel(QWidget *parent = 0);
    ~AccountPanel();

public slots:
    void loadingStart(Account *acc);
    void loadingStop(Account *acc);
    void fetchFeedDone(int status, Account *acc);
    void fetchFriendDone(int status, Account *acc);
    void onCustomMenuRequested(const QPoint & pos);

private slots:
    void updateFeedWindow(Account *acc);
    bool removeUnusedFeeds(Account *acc, QBoxLayout *layout);
    void updateFeedStat(int count);
    void updateBannerWindow(Account *acc);
    void updateNoteWindow(Account *acc);
    bool removeUnusedNotes(Account *acc, QBoxLayout *layout);
    void updateNoteStat(int count);

    void updateFriendWindow(Account *acc);
    bool removeUnusedFriends(Account *acc, QBoxLayout *layout);
    void updateFriendStat(int count);

    void switchToFeedWindow();
    void switchToNoteWindow();
    void switchToFriendWindow();
    void switchToWindow(int which);
    void haveNNewNoteClicked();
    void showAccountBannerArea();
    void showFeedStatusArea();
    void showNoteStatusArea();
    void showFriendStatusArea();

    QString getSelectedFriendUid();
    void gotoFriendHome();
    void getFriendInfo();
    void sayHello();
    void startChat();
    
    void sendMsg(QString uid, QString msg);

protected:
    virtual void mouseMoveEvent ( QMouseEvent * event );

private:
    Ui::AccountPanel ui_win;
    //SearchBar *search_win;
    QMovie *loading_movie;
    //QMovie *loading2; // for letter and notice
    QStack<int>  loadRunStack; //for stacked run loading stack
    FeedGearbox  * gearBox;
    QBoxLayout *feed_layout;
    FeedGearbox  * noteGearBox;
    QBoxLayout *note_layout;
    FeedGearbox * friendGearBox;
    QBoxLayout *friend_layout;
    QLabel  *loading_win;
    enum {WIN_MIN, WIN_FEED, WIN_NOTE, WIN_FRIEND, WIN_MAX};
    Account *account;
    FriendItemDelegate * friendItemDelegate;
    char oldPanelIndex; //for have n note click
    short noteCount;//
    QMenu *friendListViewMenu;
    QMap<QString, ChatWindow*> chatWindows;
    QMap<QString, QString> chatUsers;
};

#endif
