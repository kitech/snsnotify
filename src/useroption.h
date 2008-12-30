#ifndef USER_OPTION_H
#define USER_OPTION_H

#include <QtCore>
#include <QtNetwork>
#include <QtConcurrentRun>
#include <QFutureWatcher>

class SystemOption;
class SiteConst;
class Account;
class FeedRecord;
class FriendListModel;

//a user and a site combine
class Account : public QThread
{
    Q_OBJECT;
public:
    Account(QString userName, QString password, int siteId, QObject *parent = 0);
    ~Account();

    QString u16Message();
    void clearTempFeeds();
    void clearTempFriends();
    void clearTempNotes();
    
    void run();

    QString findFriendNameByUid(QString uid);

    //QtConcurrent
    QFutureWatcher<int> fetchFeedWacher;
    QFutureWatcher<int> fetchFriendWacher;
    
public:
    enum {LOG_NON, LOG_RESOLVE_SECCODE, LOG_LOGGING, LOG_LOGGED, LOG_ERROR}; // login status
    enum {T_NON, T_RESOLVE_SECCODE, T_LOGIN, T_FEED, T_LETTER,  T_MAX};

    QString userName;
    QString password;    

    SiteConst *sc;
    int loginStatus;
    int taskNo;  // 当前http connection在做什么工作。 

    QList<FeedRecord*> feeds;
    QList<FeedRecord*> tmpFeeds;
    QList<FeedRecord*> newFeeds;
    QList<FeedRecord*> delFeeds;

    QList<FeedRecord*> notes;
    QList<FeedRecord*> tmpNotes;
    QList<FeedRecord*> newNotes;
    QList<FeedRecord*> delNotes;

    FriendListModel *friendListModel;
    QList<FeedRecord*> friends;
    QList<FeedRecord*> tmpFriends;
    QList<FeedRecord*> newFriends;
    QList<FeedRecord*> delFriends;

    QTimer updateFeedTimer;
    QTimer updateFriendTimer;

signals:
    void fetchFeed(Account *acc);

    void loginStart(Account *acc);
    void loginDone(int status, Account *acc);
    void sendMessage(QString msg);
    void fetchFeedStart(Account *acc);
    void fetchFeedDone(int status, Account *acc);

    void fetchFriend(Account *acc);
    void fetchFriendStart(Account *acc);
    void fetchFriendDone(int status, Account *acc);    

public slots:
    void startFetchFeed();
    void startFetchFriend();
    void startLogin();
    void loginDone(int status);

private slots:
    void updateFeedTimerTimeout();
    void updateFriendTimerTimeout();
    void fetchFeedFinished();
    void fetchFriendFinished();

private:
    int findNewFeed(Account *acc);
    int findNewNote(Account *acc);
    int findNewFriend(Account *acc);
};

class Users : public QObject
{
    Q_OBJECT;
public:
    Users(QObject *parent = 0);
    ~Users();

    Account * find(QString userName, int siteId);

    int loggedAccountCount();

public slots:
    //depcreated method
    bool login(QString userName, QString password, int siteId);
    bool login(QString userName, QString password, 
               QList<int> siteIds);
    bool login(Account *acc);
    void accountLoginFinished();

    bool fetchFeed(Account *acc);

signals:
    void loginStart(Account *acc);
    void loginDone(int status, Account *acc);
    void sendMessage(QString msg);
    void fetchFeedDone(int status, Account *acc);
    void fetchFriendDone(int status, Account *acc);
    void fetchFeedStart(Account *acc);
    void fetchFriendStart(Account *acc);

public:
    ////////
    QVector<Account*> users;
};

///////////
extern Users *users;
void dumpHttpRequestHeader(const QHttpRequestHeader &header);
void dumpHttpResponseHeader(const QHttpResponseHeader &header);

#endif

