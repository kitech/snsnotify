
#include "utils.h"
#include "systemoption.h"
#include "useroption.h"
#include "feeditemwidget.h"
#include "feedgearbox.h"

//#include "searchbar.h"
#include "frienditemwidget.h"
#include "frienditemdelegate.h"
#include "friendlistmodel.h"
#include "chatwindow.h"

#include "accountpanel.h"

AccountPanel::AccountPanel(QWidget *parent)
    : QWidget(parent)
{
    this->ui_win.setupUi(this);
    QObject::connect(this->ui_win.listView, SIGNAL(customContextMenuRequested(const QPoint &)),
                     this, SLOT(onCustomMenuRequested(const QPoint&)));
    this->friendListViewMenu = NULL;
    this->oldPanelIndex = -1;
    this->noteCount = 0;

    // this->search_win = new SearchBar();
    // QBoxLayout *layout = static_cast<QBoxLayout*>(this->layout());
    // layout->insertWidget(0, this->search_win);
    this->ui_win.toolButton_5->setIcon(QIcon(so->iconPath+"/feed.png"));
    this->ui_win.toolButton_6->setIcon(QIcon(so->iconPath+"/mail-message.png"));
    this->ui_win.toolButton_8->setIcon(QIcon(so->iconPath+"/mail-message.png"));
    this->ui_win.toolButton_4->setIcon(QIcon(so->iconPath+"/ico_hy.gif"));
    this->account = NULL;
    this->friendItemDelegate = new FriendItemDelegate();
    this->ui_win.listView->setItemDelegate(this->friendItemDelegate);
    
    QMovie *movie = new QMovie(so->iconPath + "/loading.gif", QByteArray(), this);
    this->loading_movie = movie;
    this->loading_win = new QLabel();
    this->loading_win->setMovie(movie);
    //this->ui_win.tabWidget->setCornerWidget(this->loading_win);
    //this->ui_win.scrollArea->setCornerWidget(this->loading_win);
    movie->setScaledSize(QSize(20,20));
    this->ui_win.label_4->setMovie(movie);
    movie->start();
    //movie->stop();
    movie->setPaused(true);

    this->feed_layout = static_cast<QBoxLayout*>(this->ui_win.scrollAreaWidgetContents->layout());
    this->gearBox = new FeedGearbox(this->feed_layout, this);
    q_debug()<<"now item count:"<<this->feed_layout->count();
    if(this->feed_layout->count() > 0) {
        QLayoutItem *child;
        while ((child = this->feed_layout->takeAt(0)) != 0) {
            delete child;
        }
    }
    q_debug()<<"now item count:"<<this->feed_layout->count();

    this->note_layout = static_cast<QBoxLayout*>(this->ui_win.scrollAreaWidgetContents_3->layout());
    this->noteGearBox = new FeedGearbox(this->note_layout, this);
    q_debug()<<"now item count:"<<this->note_layout->count();
    if(this->note_layout->count() > 0) {
        QLayoutItem *child;
        while ((child = this->note_layout->takeAt(0)) != 0) {
            delete child;
        }
    }
    q_debug()<<"now item count:"<<this->note_layout->count();

    // this->friend_layout = static_cast<QBoxLayout*>(this->ui_win.scrollAreaWidgetContents_5->layout());
    // this->friendGearBox = new FeedGearbox(this->friend_layout, this);
    // q_debug()<<"now item count:"<<this->friend_layout->count();
    // if(this->friend_layout->count() > 0) {
    //     QLayoutItem *child;
    //     while ((child = this->friend_layout->takeAt(0)) != 0) {
    //         delete child;
    //     }
    // }
    // q_debug()<<"now item count:"<<this->friend_layout->count();

    QObject::connect(users, SIGNAL(fetchFeedStart(Account*)), this, SLOT(loadingStart(Account *)));
    QObject::connect(users, SIGNAL(fetchFriendStart(Account*)), this, SLOT(loadingStart(Account *)));
    QObject::connect(this->ui_win.toolButton_5, SIGNAL(clicked()), this, SLOT(switchToFeedWindow()));
    QObject::connect(this->ui_win.toolButton_6, SIGNAL(clicked()), this, SLOT(switchToNoteWindow()));
    QObject::connect(this->ui_win.toolButton_4, SIGNAL(clicked()), this, SLOT(switchToFriendWindow()));
    QObject::connect(this->ui_win.toolButton_8, SIGNAL(clicked()), this, SLOT(haveNNewNoteClicked()));
    QObject::connect(this->ui_win.toolButton_3, SIGNAL(clicked()), this, SLOT(showAccountBannerArea()));

    QObject::connect(this->ui_win.toolButton_2, SIGNAL(clicked()), this, SLOT(showFeedStatusArea()));
    QObject::connect(this->ui_win.toolButton_7, SIGNAL(clicked()), this, SLOT(showNoteStatusArea()));
    QObject::connect(this->ui_win.toolButton_9, SIGNAL(clicked()), this, SLOT(showFriendStatusArea()));

    ///test    
}

AccountPanel::~AccountPanel()
{

}
void AccountPanel::loadingStart(Account *acc)
{
    // q_debug()<<"starting......"<<acc;    
    //q_debug()<<this->loadRunStack;
    //this->loading_win->setMovie(this->loading_movie);
    //this->loading_movie->start();
    this->loading_movie->setPaused(false);
    this->loadRunStack.push(1);
    //q_debug()<<this->loadRunStack;
}
void AccountPanel::loadingStop(Account *acc)
{
    //q_debug()<<this->loadRunStack;
    if(!this->loadRunStack.isEmpty()) {
        int ch = this->loadRunStack.pop();
        if(this->loadRunStack.isEmpty()) {
            //this->loading_movie->stop();
            this->loading_movie->setPaused(true);
        }
    }else{
        q_debug()<<"Warning: stack empty";
    }
    //this->loading_win->setMovie(NULL);
    //q_debug()<<this->loadRunStack;
}

void AccountPanel::fetchFeedDone(int status, Account *acc)
{
    q_debug()<<"Got it";
    if(status == SiteConst::FETCH_FEED_OK) {
        q_debug()<<"fetch feed ok:"<< acc->newFeeds.count();
        //if(acc->newFeeds.count() > 0 && this->newFeedNotice) {
            //newFeedNotice is a switch
            //this->showTrayMessage(acc->newFeeds.count());
        //}
        this->updateBannerWindow(acc);
        this->updateFeedWindow(acc);
        this->updateNoteWindow(acc);
    }else{
        q_debug()<<"fetch feed error: ";
    }
    this->loadingStop(acc);
}

void AccountPanel::updateFeedWindow(Account *acc)
{
    Q_ASSERT(acc != NULL);

    int cnt = 0;
    FeedRecord *fr = NULL;
    FeedItemWidget *fiw = NULL;
    QBoxLayout *layout = this->feed_layout;

    if(acc->delFeeds.count() > 0) {
        this->removeUnusedFeeds(acc, layout);
    }
    cnt = acc->newFeeds.count();
    //q_debug()<<"now item count:"<<layout->count();
    for(int i = cnt - 1 ;i >= 0; i--) {
        fr = acc->newFeeds.at(i);
        //fiw = new FeedItemWidget(fr);        
        //layout->insertWidget(0, fiw);
        this->gearBox->addFeed(fr);
    }
    this->updateFeedStat(cnt);
    //q_debug()<<"now item count:"<<layout->count();
}

bool AccountPanel::removeUnusedFeeds(Account *acc, QBoxLayout *layout)
{
    Q_ASSERT(acc != NULL);
    Q_ASSERT(layout != NULL);
    if(acc->delFeeds.count() == 0) {
        return true;
    }
    FeedRecord *fr = NULL;
    FeedItemWidget *fiw = NULL;
    for(int i = 0 ; i < acc->delFeeds.count(); i++) {
        fr = acc->delFeeds.at(i);
        int wc = layout->count();
        //q_debug()<<"item count:"<<wc;
        for(int j = 0 ; j < wc; j++) {
            fiw = static_cast<FeedItemWidget*>(layout->itemAt(j)->widget());
            if(fiw == NULL) {
                //q_debug()<<"Warning fiw NULL";
                continue;
            }
            if(fiw->equalsTo(fr)) {
                layout->removeWidget(fiw);
                delete fiw; fiw = NULL;
                layout->update();
                break;
            }
        }
    }
    this->updateFeedStat( -1 );

    //debug code
    int wc = layout->count();
    //q_debug()<<"now item count:"<<wc;
    for(int i = 0 ; i < wc ; i ++) {
        fiw = static_cast<FeedItemWidget*>(layout->itemAt(i)->widget());
        if(fiw == NULL) {
            q_debug()<<"Warning fiw NULL: "<<i;
        }
    }

    return true;
}

void AccountPanel::updateFeedStat(int count)
{
    QLayout *layout = this->feed_layout;
    if(count == -1) {
        this->ui_win.label->setText(QString(tr("Total: <B>%1</B>  ")).arg(layout->count()));
    }else{
        this->ui_win.label->setText(QString(tr("Total: <B>%1</B>  ")).arg(layout->count() + count));
    }
}
void AccountPanel::updateBannerWindow(Account *acc)
{
    this->ui_win.webView->setUrl(acc->sc->photourl);
    this->ui_win.label_2->setText(acc->userName);
    this->ui_win.label_3->setText(acc->sc->signtext);
    if(acc->sc->activetext.length() > 0) {
        this->ui_win.label_2->setToolTip(acc->sc->activetext);
    }
}

void AccountPanel::switchToFeedWindow()
{
    this->setWindowTitle(tr("SNS Live Messager [Feed]"));
    this->switchToWindow(WIN_FEED);
}
void AccountPanel::switchToFriendWindow()
{
    this->switchToWindow(WIN_FRIEND);
}
void AccountPanel::switchToNoteWindow()
{
    this->switchToWindow(WIN_NOTE);
}

void AccountPanel::switchToWindow(int which)
{
    switch(which) {
    case WIN_FEED:
        this->ui_win.stackedWidget->setCurrentIndex(0);
        break;
    case WIN_NOTE:
        this->ui_win.stackedWidget->setCurrentIndex(1);
        break;
    case WIN_FRIEND:
        this->ui_win.stackedWidget->setCurrentIndex(2);
        break;
    default:
        q_debug()<<"Unknown or unimpled WIN_* type";
    };
}
void AccountPanel::haveNNewNoteClicked()
{
    if(this->noteCount == 0) {
        return;
    }
    if(this->oldPanelIndex == -1) {
        //this->oldPanelIndex = this->ui_win.stackedWidget->currentIndex();
        if(this->ui_win.stackedWidget->currentIndex() == 1) {
        }else{
            this->oldPanelIndex = this->ui_win.stackedWidget->currentIndex();
            this->switchToWindow(WIN_NOTE);
        }
    }else{
        this->ui_win.stackedWidget->setCurrentIndex(this->oldPanelIndex);
        this->oldPanelIndex = -1;
    }
}

void AccountPanel::showAccountBannerArea()
{
    QToolButton *btn = this->ui_win.toolButton_3;

    if(btn->arrowType() == Qt::UpArrow) {
        btn->setArrowType(Qt::DownArrow);
        this->ui_win.widget->setVisible(false);
    }else{
        btn->setArrowType(Qt::UpArrow);
        this->ui_win.widget->setVisible(true);
    }
}

void AccountPanel::showFeedStatusArea()
{
    QToolButton *btn = this->ui_win.toolButton_2;

    if(btn->arrowType() == Qt::UpArrow) {
        btn->setArrowType(Qt::DownArrow);
        this->ui_win.widget_2->setVisible(true);
    }else{
        btn->setArrowType(Qt::UpArrow);
        this->ui_win.widget_2->setVisible(false);
    }    
}
void AccountPanel::showNoteStatusArea()
{
    QToolButton *btn = this->ui_win.toolButton_7;

    if(btn->arrowType() == Qt::UpArrow) {
        btn->setArrowType(Qt::DownArrow);
        this->ui_win.widget_3->setVisible(true);
    }else{
        btn->setArrowType(Qt::UpArrow);
        this->ui_win.widget_3->setVisible(false);
    }
}
void AccountPanel::showFriendStatusArea()
{
    QToolButton *btn = this->ui_win.toolButton_9;

    if(btn->arrowType() == Qt::UpArrow) {
        btn->setArrowType(Qt::DownArrow);
        this->ui_win.widget_4->setVisible(true);
    }else{
        btn->setArrowType(Qt::UpArrow);
        this->ui_win.widget_4->setVisible(false);
    }
}

void AccountPanel::updateNoteWindow(Account *acc)
{
    Q_ASSERT(acc != NULL);

    int cnt = 0;
    FeedRecord *fr = NULL;
    FeedItemWidget *fiw = NULL;
    QBoxLayout *layout = this->note_layout;

    if(acc->delNotes.count() > 0) {
        this->removeUnusedNotes(acc, layout);
    }
    cnt = acc->newNotes.count();
    //q_debug()<<"now item count:"<<layout->count();
    for(int i = cnt - 1 ;i >= 0; i--) {
        fr = acc->newNotes.at(i);
        //fiw = new FeedItemWidget(fr);        
        //layout->insertWidget(0, fiw);
        this->noteGearBox->addFeed(fr);
    }
    this->updateNoteStat(cnt);
    //q_debug()<<"now item count:"<<layout->count();
}

bool AccountPanel::removeUnusedNotes(Account *acc, QBoxLayout *layout)
{
    Q_ASSERT(acc != NULL);
    Q_ASSERT(layout != NULL);
    if(acc->delNotes.count() == 0) {
        return true;
    }
    FeedRecord *fr = NULL;
    FeedItemWidget *fiw = NULL;
    for(int i = 0 ; i < acc->delNotes.count(); i++) {
        fr = acc->delNotes.at(i);
        int wc = layout->count();
        //q_debug()<<"item count:"<<wc;
        for(int j = 0 ; j < wc; j++) {
            fiw = static_cast<FeedItemWidget*>(layout->itemAt(j)->widget());
            if(fiw == NULL) {
                //q_debug()<<"Warning fiw NULL";
                continue;
            }
            if(fiw->equalsTo(fr)) {
                layout->removeWidget(fiw);
                delete fiw; fiw = NULL;
                layout->update();
                break;
            }
        }
    }
    this->updateNoteStat( -1 );

    //debug code
    int wc = layout->count();
    //q_debug()<<"now item count:"<<wc;
    for(int i = 0 ; i < wc ; i ++) {
        fiw = static_cast<FeedItemWidget*>(layout->itemAt(i)->widget());
        if(fiw == NULL) {
            q_debug()<<"Warning fiw NULL: "<<i;
        }
    }

    return true;
}

void AccountPanel::updateNoteStat(int count)
{
    QLayout *layout = this->note_layout;
   
    if(count == -1) {
        this->noteCount = layout->count();
        this->ui_win.label_6->setText(QString(tr("Total: <B>%1</B>  ")).arg(layout->count()));
    }else{
        this->noteCount = layout->count() + count;
        this->ui_win.label_6->setText(QString(tr("Total: <B>%1</B>  ")).arg(layout->count() + count));
    }
    this->ui_win.toolButton_8->setText(QString(tr("Have %1 Notice")).arg(this->noteCount));
}

void AccountPanel::fetchFriendDone(int status, Account *acc)
{
    q_debug()<<"run here";
    q_debug()<<"Got it";
    if(status == SiteConst::FETCH_FEED_OK) {
        q_debug()<<"fetch friend ok:"<< acc->newFriends.count();
        //if(acc->newFeeds.count() > 0 && this->newFeedNotice) {
            //newFeedNotice is a switch
            //this->showTrayMessage(acc->newFeeds.count());
        //}
        //this->updateFriendWindow(acc);
        if(this->account == NULL) {
            q_debug()<<"here";
            this->account = acc;
            this->ui_win.listView->setModel((QAbstractItemModel*)this->account->friendListModel);
        }
        this->updateFriendStat(-1);
    }else{
        q_debug()<<"fetch friend error: ";
    }
    this->loadingStop(acc);
}

//no used
void AccountPanel::updateFriendWindow(Account *acc)
{
    Q_ASSERT(acc != NULL);

    int cnt = 0;
    FeedRecord *fr = NULL;
    FeedItemWidget *fiw = NULL;
    QBoxLayout *layout = this->friend_layout;

    if(acc->delFriends.count() > 0) {
        this->removeUnusedFriends(acc, layout);
    }
    cnt = acc->newFriends.count();
    //q_debug()<<"now item count:"<<layout->count();
    for(int i = cnt - 1 ;i >= 0; i--) {
        fr = acc->newFriends.at(i);
        //fiw = new FeedItemWidget(fr);        
        //layout->insertWidget(0, fiw);
        this->friendGearBox->addFeed(fr);
    }
    this->updateFriendStat(cnt);
    //q_debug()<<"now item count:"<<layout->count();    
}

//no used
bool AccountPanel::removeUnusedFriends(Account *acc, QBoxLayout *layout)
{
    Q_ASSERT(acc != NULL);
    Q_ASSERT(layout != NULL);
    if(acc->delFriends.count() == 0) {
        return true;
    }
    FeedRecord *fr = NULL;
    FeedItemWidget *fiw = NULL;
    for(int i = 0 ; i < acc->delFriends.count(); i++) {
        fr = acc->delFriends.at(i);
        int wc = layout->count();
        //q_debug()<<"item count:"<<wc;
        for(int j = 0 ; j < wc; j++) {
            fiw = static_cast<FeedItemWidget*>(layout->itemAt(j)->widget());
            if(fiw == NULL) {
                //q_debug()<<"Warning fiw NULL";
                continue;
            }
            if(fiw->equalsTo(fr)) {
                layout->removeWidget(fiw);
                delete fiw; fiw = NULL;
                layout->update();
                break;
            }
        }
    }
    this->updateFriendStat( -1 );

    return true;
}

void AccountPanel::updateFriendStat(int count)
{
    int row = ((QAbstractItemModel*)this->account->friendListModel)->rowCount();
    this->ui_win.label_8->setText(QString(tr("Total: <B>%1</B>  ")).arg(row));
}

void AccountPanel::onCustomMenuRequested(const QPoint & pos)
{
    //q_debug()<<"menu popup";
    if(this->friendListViewMenu == NULL) {
        this->friendListViewMenu = new QMenu(this);
        QAction *action = new QAction(tr("Go friend's &Home page"), this);
        this->friendListViewMenu->addAction(action);
        QObject::connect(action, SIGNAL(triggered()), this, SLOT(gotoFriendHome()));

        // action = new QAction(tr("View friend's &info"), this);
        // this->friendListViewMenu->addAction(action);
        // QObject::connect(action, SIGNAL(triggered()), this, SLOT(getFriendInfo()));

        // action = new QAction(tr("&Say Hello"), this);
        // this->friendListViewMenu->addAction(action);
        // QObject::connect(action, SIGNAL(triggered()), this, SLOT(getFriendInfo()));        

        action = new QAction(tr("Send &message"), this);
        this->friendListViewMenu->addAction(action);
        QObject::connect(action, SIGNAL(triggered()), this, SLOT(startChat()));
    }
    this->friendListViewMenu->popup(this->ui_win.listView->mapToGlobal(pos));
}

QString AccountPanel::getSelectedFriendUid()
{
    QString uid;
    QItemSelectionModel *sim = this->ui_win.listView->selectionModel();
    QModelIndexList mil = sim->selectedIndexes();
    QModelIndex index;
    if(mil.count() > 0) {
        index = mil.at(0);

        QString homePage;
        FeedRecord *rec = NULL;
        QVariant dv;

        dv = index.model()->data(index, Qt::EditRole);
        rec = (FeedRecord*)qVariantValue<void*>(dv);

        //q_debug()<<rec<<index;
        uid = rec->fuid;
    }
    return uid;
}

void AccountPanel::gotoFriendHome()
{
    QString uid;
    QString homePage;

    uid = this->getSelectedFriendUid();
    if(!uid.isEmpty()) {
        homePage = this->account->sc->homePageUrl(uid);
        q_debug()<<homePage;
        QDesktopServices::openUrl(homePage);
    }
}
void AccountPanel::getFriendInfo()
{

}
void AccountPanel::sayHello()
{
    QString uid = this->getSelectedFriendUid();
    QString fusername = this->account->findFriendNameByUid(uid);
    if(!uid.isEmpty()) {
        //this->account->sc->sayHello(uid);
    }    
    
}

void AccountPanel::startChat()
{
    QString uid;
    ChatWindow *cw = NULL;

    uid = this->getSelectedFriendUid();
    if(!uid.isEmpty()) {
        if(this->chatWindows.contains(uid)) {
            //active it
            cw = this->chatWindows[uid];
            cw->activateWindow();
        }else{
            cw = new ChatWindow(uid, this->account->findFriendNameByUid(uid), this );
            QObject::connect(cw, SIGNAL(sendMsg(QString, QString)), 
                             this, SLOT(sendMsg(QString, QString)));
            this->chatWindows.insert(uid, cw);
        }
        cw->show();
    }    
}
void AccountPanel::sendMsg(QString uid, QString msg)
{
    q_debug()<<"";
    QString fusername = this->account->findFriendNameByUid(uid);
    this->account->sc->sendMsg(fusername, msg);
}

void AccountPanel::mouseMoveEvent ( QMouseEvent * event )
{
    QPoint mouse_gpos = event->globalPos();
    QRect  rect = this->geometry();
    QPoint this_gpos = this->mapToGlobal(this->pos());
    int  distance = 0;

    this_gpos.setY(this_gpos.y() + rect.height());
    distance = this_gpos.y() - mouse_gpos.y();
    
    //q_debug()<<mouse_gpos<<this_gpos<<distance;
    
    if(distance < -10 || distance > 10) {
        return;
    }
    if(this->ui_win.stackedWidget->currentIndex() == 0) {
        if(!this->ui_win.widget_2->isVisible()) {
            this->showFeedStatusArea();
        }
    }else if(this->ui_win.stackedWidget->currentIndex() == 1) {
        if(!this->ui_win.widget_3->isVisible()) {
            this->showNoteStatusArea();
        }        
    }else if(this->ui_win.stackedWidget->currentIndex() == 2) {
        if(!this->ui_win.widget_4->isVisible()) {
            this->showFriendStatusArea();
        }
    }else{
        
    }
}


