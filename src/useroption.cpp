#include "utils.h"
#include "systemoption.h"
#include "useroption.h"
#include "friendlistmodel.h"

Users *users = new Users();

//some functors for data fetch
int functor_fetch_feed(SiteConst *sc)
{
    return sc->fetchFeedData();
}
int functor_fetch_friend(SiteConst *sc)
{
    return sc->fetchFriendData();
}
//////////////////////
////
///////////////////
Account::Account(QString userName, QString password, int siteId, QObject *parent) 
    : QThread(parent)
{
    Q_ASSERT(siteId > SiteConst::SITE_MIN && siteId < SiteConst::SITE_MAX);

    sc = so->getSite(siteId);
    Q_ASSERT(sc != NULL);
    this->userName = userName;
    this->password = password;
    sc->username = userName;
    sc->password = password;

    loginStatus = LOG_NON;

    this->friendListModel = new FriendListModel();

    QObject::connect(this, SIGNAL(loginStart(Account *)), this->sc, SLOT(login(Account *)));
    QObject::connect(this->sc, SIGNAL(loginDone(int)), this, SLOT(loginDone(int)));

    this->updateFeedTimer.setInterval(so->updateIntval*1000);
    QObject::connect(&this->updateFeedTimer, SIGNAL(timeout()), this, SLOT(updateFeedTimerTimeout()));

    this->updateFriendTimer.setInterval(so->updateIntval*1000);
    QObject::connect(&this->updateFriendTimer, SIGNAL(timeout()), this, SLOT(updateFriendTimerTimeout()));

    QObject::connect(&this->fetchFeedWacher, SIGNAL(finished()), this, SLOT(fetchFeedFinished()));
    QObject::connect(&this->fetchFriendWacher, SIGNAL(finished()), this, SLOT(fetchFriendFinished()));
}

Account::~Account()
{
    
}

void Account::startFetchFeed()
{
    emit fetchFeedStart(this);
    QFuture<int> future = QtConcurrent::run(functor_fetch_feed, this->sc);
    this->fetchFeedWacher.setFuture(future);
    if(!this->updateFeedTimer.isActive()) {
        this->updateFeedTimer.start();
    }
}
void Account::updateFeedTimerTimeout()
{
    q_debug()<<"fetch feed signaled";
    //emit fetchFeed(this);
    this->startFetchFeed();
}
void Account::updateFriendTimerTimeout()
{
    q_debug()<<"fetch friend signaled";
    this->startFetchFriend();
}

void Account::clearTempFeeds()
{
    //del feeds, new feeds, tmp feeds
    FeedRecord *fr = NULL;

    //new Feeds must be in feeds membor
    this->newFeeds.clear();
    
    for(int i = 0; i < this->delFeeds.count(); i++) {
        fr = this->delFeeds.at(i);
        delete fr;
        fr = NULL;
    }
    this->delFeeds.clear();

    for(int i = 0 ; i < this->tmpFeeds.count(); i++) {
        fr = this->tmpFeeds.at(i);
        delete fr;
        fr = NULL;
    }
    this->tmpFeeds.clear();
}

void Account::clearTempNotes()
{
    //del feeds, new feeds, tmp feeds
    FeedRecord *fr = NULL;

    //new Feeds must be in feeds membor
    this->newNotes.clear();
    
    for(int i = 0; i < this->delNotes.count(); i++) {
        fr = this->delFeeds.at(i);
        delete fr;
        fr = NULL;
    }
    this->delNotes.clear();

    for(int i = 0 ; i < this->tmpNotes.count(); i++) {
        fr = this->tmpNotes.at(i);
        delete fr;
        fr = NULL;
    }
    this->tmpNotes.clear();
}

void Account::clearTempFriends()
{
    //del feeds, new feeds, tmp feeds
    FeedRecord *fr = NULL;

    //new Feeds must be in feeds membor
    this->newFriends.clear();
    
    for(int i = 0; i < this->delFriends.count(); i++) {
        fr = this->delFriends.at(i);
        delete fr;
        fr = NULL;
    }
    this->delFriends.clear();

    for(int i = 0 ; i < this->tmpFriends.count(); i++) {
        fr = this->tmpFriends.at(i);
        delete fr;
        fr = NULL;
    }
    this->tmpFriends.clear();
}


QString Account::u16Message()
{
    QString umsg;
    // if(!this->loginErrorMessage.isEmpty()) {
    //     umsg = this->sc->codec->toUnicode(this->loginErrorMessage.toAscii());
    // }
    return umsg;
}

void Account::run()
{
    //loginStatus = this->sc->login();    
}
 
void Account::startLogin()
{
    emit this->loginStart(this);
}
void Account::loginDone(int status)
{
    q_debug()<<"hahaha:"<<status;
    emit this->loginDone(status, this);
}
void Account::fetchFeedFinished()
{
    q_debug()<<"";

    TidyDoc doc = NULL;
    TidyNode n = NULL;
    QString ns ;
    void *state_obj = NULL;
    FeedRecord *rec = NULL;

    this->clearTempFeeds();
    this->tmpFeeds.clear();

    doc = this->sc->create_feed_document_tree();
    if(doc == NULL) {
        emit fetchFeedDone(SiteConst::FETCH_FEED_ERROR, this);
        return ;
        Q_ASSERT(doc != NULL);
    }
    state_obj = this->sc->create_parser_state_data();
    for(n = this->sc->first_feed(state_obj, doc); n != NULL; 
        n = this->sc->next_feed(state_obj, doc, n)) {
        rec = this->sc->parse_feed(state_obj, doc, n);
        if(rec == NULL) {
            q_debug()<<"NULL feed item!!!";
            continue;
            Q_ASSERT(rec != NULL);
        }
        rec->account = this;
        //q_debug()<<"FEED: "<<rec->md5StringSum()<<rec->content;
        this->tmpFeeds.append(rec);
        Q_ASSERT(rec != NULL);
    }    
    this->sc->free_parser_state_data(state_obj);
    this->sc->free_feed_document_tree(doc);
    doc = NULL;
    //find new
    this->findNewFeed(this);

    ///////////notice
    this->clearTempNotes();
    this->tmpNotes.clear();
    doc = this->sc->create_note_document_tree();
    if(doc == NULL) {
        emit fetchFeedDone(SiteConst::FETCH_FEED_ERROR, this);
        return ;
        Q_ASSERT(doc != NULL);
    }
    state_obj = this->sc->create_note_parser_state_data();
    for(n = this->sc->first_note(state_obj, doc); n != NULL; 
        n = this->sc->next_note(state_obj, doc, n)) {
        rec = this->sc->parse_note(state_obj, doc, n);
        if(rec == NULL) {
            q_debug()<<"NULL feed item!!!";
            continue;
            Q_ASSERT(rec != NULL);
        }
        rec->account = this;
        //q_debug()<<"FEED: "<<rec->md5StringSum()<<rec->content;
        this->tmpNotes.append(rec);
        Q_ASSERT(rec != NULL);
    }    
    this->sc->free_note_parser_state_data(state_obj);
    this->sc->free_note_document_tree(doc);
    doc = NULL;
    //find new note
    this->findNewNote(this);    

    emit fetchFeedDone(SiteConst::FETCH_FEED_OK, this);
}

int Account::findNewFeed(Account *acc)
{
    FeedRecord *fr = NULL, *fr2 = NULL;
    
    acc->newFeeds.clear();
    acc->delFeeds.clear();
    //#warning "Clear new Feeds and deleted feeds somewhere else. memleak here";

    q_debug()<<acc->feeds.count()<<",tmp count:"<<acc->tmpFeeds.count()
             <<",new count:"<<acc->newFeeds.count()
             <<",del count:"<<acc->delFeeds.count();

    if(acc->feeds.count() == 0) {
        for(int i = 0 ; i < acc->tmpFeeds.count(); i++) {
            fr = acc->tmpFeeds.at(i);
            acc->feeds.append(fr);
            acc->newFeeds.append(fr);
            fr = NULL;
        }
        acc->tmpFeeds.clear();
        return acc->feeds.count();
    }else{
        for(int i = 0 ; i < acc->feeds.count(); i++) {
            fr = acc->feeds.at(i);

            bool found = false;
            for(int j = 0; j < acc->tmpFeeds.count(); j++) {
                fr2 = acc->tmpFeeds.at(j);
                if(fr->equalsTo(fr2)) {
                    found = true;
                    break;
                }
            }
            if(!found) {
                acc->delFeeds.append(fr);
            }
        }
        q_debug()<<"no use feed count:"<<acc->delFeeds.count();
        for(int i = 0; i < acc->delFeeds.count(); i++) {
            fr = acc->delFeeds.at(i);
            acc->feeds.removeAll(fr);
        }
        int pos = 0;
        for(int j = 0 ; j < acc->tmpFeeds.count(); j++) {
            fr2 = acc->tmpFeeds.at(j);
            bool found = false;
            for(int i = 0; i < acc->feeds.count(); i++) {
                fr = acc->feeds.at(i);
                if(fr->equalsTo(fr2)) {
                    found = true;
                    break;
                }
            }
            if(!found) {
                acc->newFeeds.append(fr2);
                acc->feeds.insert(pos++, fr2); //对这个列表，保证新的在最前面
            }
        }
        for(int j = 0 ; j < acc->tmpFeeds.count(); j++) {
            fr2 = acc->tmpFeeds.at(j);
            if(acc->feeds.contains(fr2)) {
                
            }else{
                delete fr2;
            }
        }
        acc->tmpFeeds.clear();
    }

    return acc->newFeeds.count();
}

int Account::findNewNote(Account *acc)
{
    FeedRecord *fr = NULL, *fr2 = NULL;
    // QDateTime nowDate = QDateTime::currentDateTime();
    // QDate lastDate = nowDate.date().addYears(-20) ; // 20 years later

    acc->newNotes.clear();
    acc->delNotes.clear();
    //#warning "Clear new Feeds and deleted feeds somewhere else. memleak here";

    q_debug()<<acc->notes.count()<<",tmp count:"<<acc->tmpNotes.count()
             <<",new count:"<<acc->newNotes.count()
             <<",del count:"<<acc->delNotes.count();

    if(acc->notes.count() == 0) {
        for(int i = 0 ; i < acc->tmpNotes.count(); i++) {
            fr = acc->tmpNotes.at(i);
            acc->notes.append(fr);
            acc->newNotes.append(fr);
            fr = NULL;
        }
        acc->tmpNotes.clear();
        return acc->notes.count();
    }else{
        for(int i = 0 ; i < acc->notes.count(); i++) {
            fr = acc->notes.at(i);

            bool found = false;
            for(int j = 0; j < acc->tmpNotes.count(); j++) {
                fr2 = acc->tmpNotes.at(j);
                if(fr->equalsTo(fr2)) {
                    found = true;
                    break;
                }
            }
            if(!found) {
                acc->delNotes.append(fr);
            }
        }
        q_debug()<<"no use feed count:"<<acc->delNotes.count();
        for(int i = 0; i < acc->delNotes.count(); i++) {
            fr = acc->delNotes.at(i);
            acc->notes.removeAll(fr);
        }
        int pos = 0;
        for(int j = 0 ; j < acc->tmpNotes.count(); j++) {
            fr2 = acc->tmpNotes.at(j);
            bool found = false;
            for(int i = 0; i < acc->notes.count(); i++) {
                fr = acc->notes.at(i);
                if(fr->equalsTo(fr2)) {
                    found = true;
                    break;
                }
            }
            if(!found) {
                acc->newNotes.append(fr2);
                acc->notes.insert(pos++, fr2); //对这个列表，保证新的在最前面
            }
        }
        for(int j = 0 ; j < acc->tmpNotes.count(); j++) {
            fr2 = acc->tmpNotes.at(j);
            if(acc->notes.contains(fr2)) {
                
            }else{
                delete fr2;
            }
        }
        acc->tmpNotes.clear();
    }

    return acc->newNotes.count();
}

void Account::startFetchFriend()
{
    emit fetchFriendStart(this);
    QFuture<int> future = QtConcurrent::run(functor_fetch_friend, this->sc);
    this->fetchFriendWacher.setFuture(future);
    if(!this->updateFriendTimer.isActive()) {
        this->updateFriendTimer.start();
    }
}
void Account::fetchFriendFinished()
{
    q_debug()<<"run here";

    TidyDoc doc = NULL;
    TidyNode n = NULL;
    QString ns ;
    void *state_obj = NULL;
    FeedRecord *rec = NULL;

    this->clearTempFriends();
    this->tmpFriends.clear();

    doc = this->sc->create_friend_document_tree();
    if(doc == NULL) {
        emit fetchFriendDone(SiteConst::FETCH_FEED_ERROR, this);
        return ;
        Q_ASSERT(doc != NULL);
    }
    state_obj = this->sc->create_friend_parser_state_data();
    for(n = this->sc->first_friend(state_obj, doc); n != NULL; 
        n = this->sc->next_friend(state_obj, doc, n)) {
        rec = this->sc->parse_friend(state_obj, doc, n);
        if(rec == NULL) {
            q_debug()<<"NULL feed item!!!";
            continue;
            Q_ASSERT(rec != NULL);
        }
        rec->account = this;
        //q_debug()<<"FRIEND: "<<rec->md5StringSum()<<rec->content;
        this->tmpFriends.append(rec);
        Q_ASSERT(rec != NULL);
    }    
    this->sc->free_friend_parser_state_data(state_obj);
    this->sc->free_friend_document_tree(doc);
    doc = NULL;
    //find new
    this->findNewFriend(this);

    emit fetchFriendDone(SiteConst::FETCH_FEED_OK, this);
}
int Account::findNewFriend(Account *acc)
{
    FeedRecord *fr = NULL, *fr2 = NULL;

    acc->newFriends.clear();
    acc->delFriends.clear();
    //#warning "Clear new Feeds and deleted feeds somewhere else. memleak here";

    q_debug()<<acc->friends.count()<<",tmp count:"<<acc->tmpFriends.count()
             <<",new count:"<<acc->newFriends.count()
             <<",del count:"<<acc->delFriends.count();

    if(acc->friends.count() == 0) {
        for(int i = 0 ; i < acc->tmpFriends.count(); i++) {
            fr = acc->tmpFriends.at(i);
            acc->friends.append(fr);
            acc->newFriends.append(fr);
            this->friendListModel->insertItem(i, fr);
            fr = NULL;
        }
        acc->tmpFriends.clear();
        return acc->friends.count();
    }else{
        for(int i = 0 ; i < acc->friends.count(); i++) {
            fr = acc->friends.at(i);

            bool found = false;
            for(int j = 0; j < acc->tmpFriends.count(); j++) {
                fr2 = acc->tmpFriends.at(j);
                if(fr->equalsTo(fr2)) {
                    found = true;
                    break;
                }
            }
            if(!found) {
                acc->delFriends.append(fr);
            }
        }
        q_debug()<<"no use feed count:"<<acc->delFriends.count();
        for(int i = 0; i < acc->delFriends.count(); i++) {
            fr = acc->delFriends.at(i);
            acc->friends.removeAll(fr);
        }
        int pos = 0;
        for(int j = 0 ; j < acc->tmpFriends.count(); j++) {
            fr2 = acc->tmpFriends.at(j);
            bool found = false;
            for(int i = 0; i < acc->friends.count(); i++) {
                fr = acc->friends.at(i);
                if(fr->equalsTo(fr2)) {
                    found = true;
                    break;
                }
            }
            if(!found) {
                acc->newFriends.append(fr2);
                acc->friends.insert(pos++, fr2); //对这个列表，保证新的在最前面
            }
        }
        for(int j = 0 ; j < acc->tmpFriends.count(); j++) {
            fr2 = acc->tmpFriends.at(j);
            if(acc->friends.contains(fr2)) {
                
            }else{
                delete fr2;
            }
        }
        acc->tmpFriends.clear();
    }

    return acc->newFriends.count();
    return 0;
}

QString Account::findFriendNameByUid(QString uid)
{
    FeedRecord *fr = NULL;
    for(int i = 0 ; i < this->friends.count(); i++) {
        fr = this->friends.at(i);
        if(fr->fuid == uid) {
            return fr->fusername;
        }
    }
    return QString();
}

////////////////////////
//////////
/////////////////////////
Users::Users(QObject *parent)
    : QObject(parent)
{

}
Users::~Users()
{
    
}

bool Users::login(QString userName, QString password, int siteId)
{
    Account * acc = NULL;

    acc = find(userName, siteId);

    if(acc == NULL) {
        acc = new Account(userName, password, siteId);
        QObject::connect(acc, SIGNAL(finished()), this, SLOT(accountLoginFinished()));
        QObject::connect(acc, SIGNAL(loginStart(Account*)), this, SIGNAL(loginStart(Account*)));
        QObject::connect(acc, SIGNAL(loginDone(int, Account*)), 
                         this, SIGNAL(loginDone(int, Account*)));

        QObject::connect(acc, SIGNAL(fetchFeedStart(Account*)),
                         this, SIGNAL(fetchFeedStart(Account*)));        
        QObject::connect(acc, SIGNAL(fetchFeedDone(int, Account*)),
                         this, SIGNAL(fetchFeedDone(int, Account*)));

        QObject::connect(acc, SIGNAL(fetchFriendStart(Account*)),
                         this, SIGNAL(fetchFriendStart(Account*)));
        QObject::connect(acc, SIGNAL(fetchFriendDone(int, Account*)),
                         this, SIGNAL(fetchFriendDone(int, Account*)));
        //acc->start();
        acc->startLogin();
        //
        this->users.append(acc);
        
    }
    Q_ASSERT(acc != NULL);
    acc->taskNo = Account::T_LOGIN;
    if(acc->loginStatus == Account::LOG_NON) {
        acc->loginStatus = Account::LOG_LOGGING;
        this->login(acc);
    }else if(acc->loginStatus == Account::LOG_ERROR) {
        qDebug()<<"this account is log errored";
    }else{
        qDebug()<<"this account is in logging or logged";
    }

    if(acc == NULL) {
        q_debug()<<"Site not found";
    }

    return true;
}

bool Users::login(QString userName, QString password, QList<int> siteIds)
{
    Account * acc = NULL;
    int id = 0;

    for(int i = 0; i < siteIds.count(); i++) {
        this->login(userName, password, siteIds.at(i));
    }

    return true;
}

bool Users::login(Account *acc)
{
    Q_ASSERT(acc != NULL);
    return false;
}

bool Users::fetchFeed(Account *acc)
{
    Q_ASSERT(acc != NULL);
    q_debug()<<"here";

    //
    // emit fetchFeedStart(acc); //no use, this signal is from acc 2008/12/11
    acc->startFetchFeed();
    //emit fetchFriendStart(acc); //no use, this signal is from acc 2008/12/11
    acc->startFetchFriend();

    return true;
}

Account * Users::find(QString userName, int siteId)
{
    Account *acc = NULL;
 
    for(int i = 0; i < this->users.count() ; i++) {
        acc = this->users.at(i);
        if(acc->userName == userName) {
            if(acc->sc->siteId == siteId) {
                return acc;
            }
        }
        acc = NULL;
    }
  
    return acc;
}

int Users::loggedAccountCount()
{
    int c = 0;
    Account * acc = NULL;

    for(int i = 0; i < this->users.count() ; i++) {
        acc = this->users.at(i);
        if(acc->loginStatus == Account::LOG_LOGGED) c++;
        acc = NULL;
    }
  
    return c;
}

/*

  QHttp 把 setHost也当作一个请求了，问题是怎么区别它是setHost的请求呢？         

  http->setHost("www.trolltech.com");                // id == 1
  http->get(QUrl::toPercentEncoding("/index.html")); // id == 2
 */
void Users::accountLoginFinished()
{
    q_debug()<<"login done here";
    Account *acc = (Account*)sender();

    emit loginDone(acc->loginStatus, acc);
}

//tool function
void dumpHttpRequestHeader(const QHttpRequestHeader &header)
{
    q_debug()<<"Http headers: "<<header.method()<<header.path();

    QList<QPair<QString, QString> > values = header.values();
    for(int i = 0 ; i < values.count() ; i ++) {
        qDebug()<<values.at(i).first<<": "<<values.at(i).second;
    }
}
void dumpHttpResponseHeader(const QHttpResponseHeader &header)
{
    q_debug()<<"Http headers: "<<header.statusCode();
    QList<QPair<QString, QString> > values = header.values();
    for(int i = 0 ; i < values.count() ; i ++) {
        qDebug()<<values.at(i).first<<": "<<values.at(i).second;
    }
}

