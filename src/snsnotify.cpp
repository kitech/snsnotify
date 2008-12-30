#include "tidy.h"
#include "buffio.h"

#include "utils.h"
#include "snsnotify.h"
#include "login.h"
#include "systemoption.h"
#include "useroption.h"
#include "feeditemwidget.h"
#include "about.h"
#include "feedgearbox.h"
//#include "lettergearbox.h"
#include "messagehint.h"
#include "liveupdate.h"
#include "accountpanel.h"

SNSNotify::SNSNotify(QWidget *parent)
    : QMainWindow(parent)
      //    , kwCompleter(NULL)
{
    this->ui_win.setupUi(this);
    this->setupSystemTray();

    this->statBar = this->statusBar();
    this->sizeAdjusted = false;
    this->newFeedNotice = true;
    this->msgHint = NULL;

    /////////
    login_win = new Login();    
     this->setCentralWidget(login_win);

    ////////////
    QObject::connect(this, SIGNAL(wantLogin(QString, QString, QList<int>)),
                     users, SLOT(login(QString, QString,  QList<int>)));
    QObject::connect(this, SIGNAL(fetchFeed(Account*)), users, SLOT(fetchFeed(Account*)));
    QObject::connect(users, SIGNAL(loginStart(Account*)), this, SLOT(loginStart(Account*)));
    QObject::connect(users, SIGNAL(loginDone(int, Account*)), this, SLOT(loginDone(int, Account*)));
    QObject::connect(users, SIGNAL(fetchFeedDone(int, Account*)), this, SLOT(fetchFeedDone(int, Account*)));

    ///
    this->cleanupMainWindow();

    QDesktopWidget dw;
    q_debug()<<"Screen Info:"<<dw.numScreens()<<dw.screenGeometry();

    // QObject::connect(this->ui_win.pushButton, SIGNAL(clicked()),
    //                  this, SLOT(test_live_update()));

}

SNSNotify::~SNSNotify()
{
    qDebug()<<"main window destructured";
    //清除两个全局变量
    delete users;
    delete so;
}
void SNSNotify::about()
{
    About about(this);
    about.exec();
}

int SNSNotify::get_version()
{
    return 12;
}

void SNSNotify::cleanupMainWindow()
{
    //this->ui_win.scrollAreaWidgetContents->setWindowOpacity(0.0);
    
    //
    // QMovie *movie = new QMovie(so->iconPath + "/loading.gif", QByteArray(), this);
    // //this->ui_win.label_9->setMovie(movie);
    // movie->start();
    // movie->stop();
    // this->loading = movie;
}

void SNSNotify::setupSystemTray()
{
    QApplication::setWindowIcon(QIcon(so->iconPath + "/user-identity.png"));
    this->sti = new QSystemTrayIcon(QIcon(so->iconPath + "/user-identity.png"), this);
    this->sti->setToolTip(tr("SNS live messager"));
    QMenu *sysTrayMenu = new QMenu(this);
    QAction *action;
    action = new QAction(tr("&Show/Hidden main window"), this);
    action->setCheckable(true);
    action->setChecked(true);
    sysTrayMenu->addAction(action);
    // action = new QAction(tr("&Login Window"), this);
    // sysTrayMenu->addAction(action);
    // QObject::connect(action, SIGNAL(triggered()), this, SLOT(switchToLoginWindow()));

    // action = new QAction(tr("&Feed Window"), this);
    // sysTrayMenu->addAction(action);
    // QObject::connect(action, SIGNAL(triggered()), this, SLOT(switchToFeedWindow()));

    // action = new QAction(tr("Le&tter Window"), this);
    // sysTrayMenu->addAction(action);
    // QObject::connect(action, SIGNAL(triggered()), this, SLOT(switchToLetterWindow()));

    sysTrayMenu->addSeparator();
    action = new QAction(tr("&New Feed Notification"), this);
    action->setCheckable(true);
    action->setChecked(true);
    sysTrayMenu->addAction(action);
    QObject::connect(action, SIGNAL(triggered(bool)), this, SLOT(setNewFeedNotice(bool)));

    //
    action = new QAction(tr("&New Message Notification"), this);
    action->setCheckable(true);
    action->setChecked(true);
    sysTrayMenu->addAction(action);

    action = new QAction(tr("&About"), this);
    sysTrayMenu->addAction(action);
    QObject::connect(action, SIGNAL(triggered()), this, SLOT(about()));

    sysTrayMenu->addSeparator();
    action= new QAction(tr("&Exit"), this);
    sysTrayMenu->addAction(action);
    QObject::connect(action, SIGNAL(triggered()), this, SLOT(wantQuit()));
    
    this->sti->setContextMenu(sysTrayMenu);
    QObject::connect(this->sti, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                     this, SLOT(systemTrayActivated(QSystemTrayIcon::ActivationReason)));
    QObject::connect(this->sti, SIGNAL(messageClicked()), this, SLOT(systemTrayMessageClicked()));
    this->sti->show();

    //
    QToolButton *btn = NULL;

    // btn = new QToolButton(this);
    // btn->setAutoRaise(true);
    // btn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    // btn->setIconSize(QSize(23,18));
    // btn->setIcon(QIcon(so->iconPath + "/settings.gif"));
    // this->statusBar()->addPermanentWidget(btn);
    // QObject::connect(btn, SIGNAL(clicked()), this, SLOT(switchSettingWindow()));
    // btn->setToolTip(tr("Settings"));

    btn = new QToolButton(this);
    btn->setAutoRaise(true);
    btn->setPopupMode(QToolButton::MenuButtonPopup);
    btn->setMenu(sysTrayMenu);
    btn->setIcon(QIcon(so->iconPath + "/user-identity.png"));
    this->statusBar()->addPermanentWidget(btn);    
    btn->setToolTip(tr("Main Menu"));

}
void SNSNotify::systemTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason) {
    case QSystemTrayIcon::DoubleClick:
    case QSystemTrayIcon::Trigger:
        this->setVisible(!this->isVisible());
        break;
    case QSystemTrayIcon::MiddleClick:
        break;
    default:
        break;
    };
}
void SNSNotify::systemTrayMessageClicked()
{

}
void SNSNotify::showTrayMessage(int count)
{
    Q_ASSERT(count > 0);
    //#warning "is need point which account in message?"    
    QString msg = QString(tr("You have %1 messages.")).arg(count);
    this->sti->showMessage(tr("Notice:"), msg, QSystemTrayIcon::Information);
    //this->sti->setIcon();
    //this->showMessageHint(tr("Notice:"), msg);
}

void SNSNotify::wantQuit()
{
    QApplication::quit();
}
void SNSNotify::adjustMainWindowSize()
{
    if(!this->sizeAdjusted) {
        this->sizeAdjusted = true;        
        this->resize(this->width(), this->height()+60);
    }
}

void SNSNotify::loginStart(Account *acc)
{
    q_debug()<<acc;
    QObject::connect(acc->sc, SIGNAL(showLoginMessage(QString)),
                     this->login_win, SLOT(showLoginMessage(QString)));
}

void SNSNotify::loginDone(int status, Account *acc)
{
    q_debug()<<"login done";
    if(status == SiteConst::LOGIN_OK) {
        this->acc_panel = new AccountPanel();
        QObject::connect(users, SIGNAL(fetchFeedDone(int, Account*)), this->acc_panel, SLOT(fetchFeedDone(int, Account*)));
        QObject::connect(users, SIGNAL(fetchFriendDone(int, Account*)), this->acc_panel, SLOT(fetchFriendDone(int, Account*)));
        this->setCentralWidget(NULL);
        this->setCentralWidget(this->acc_panel);
        delete this->login_win;
        this->login_win = NULL;
        emit fetchFeed(acc);
        this->adjustMainWindowSize();
    }else{
        q_debug()<<"login error: ";
        this->showStatus(acc->u16Message());
        this->login_win->loginFaild(status, tr("Unknown error"));
    }    
}

void SNSNotify::fetchFeedDone(int status, Account *acc)
{
    if(status == SiteConst::FETCH_FEED_OK) {
        q_debug()<<"fetch feed ok:"<< acc->newFeeds.count();
        if(acc->newFeeds.count() > 0 && this->newFeedNotice) {
            //newFeedNotice is a switch
            this->showTrayMessage(acc->newFeeds.count());
        }
        //this->updateFeedWindow(acc);
    }else{
        q_debug()<<"fetch feed error: ";
    }
    //this->loadingStop(acc);
}

void SNSNotify::setNewFeedNotice(bool note)
{
    this->newFeedNotice = note;
}
// void SNSNotify::searchFeed()
// {
//     if(this->searchState == SE_IN) {
//         //this->statBar->showMessage(tr("We are searching, waiting..."));
//         this->showStatus(tr("We are searching, waiting..."));
//         return;
//     }
//     // if(!this->lastKeyword.isEmpty()  && 
//     //    this->lastKeyword == this->ui_win.comboBox_2->currentText()) {
//     //     //this->statBar->showMessage(tr("This keyword has searched."));
//     //     this->showStatus(tr("This keyword has searched."));
//     //     return;
//     // }
//     if(users->loggedAccountCount() <= 0 ) {
//         //this->statBar->showMessage(tr("No account logged"));
//         this->showStatus(tr("No account logged"));
//         return;
//     }

//     // if(this->ui_win.stackedWidget->currentIndex() != 1) {
//     //     //this->statBar->showMessage(tr("Switch to feed window"));
//     //     //return ;
//     //     this->switchToFeedWindow();
//     // }
//     this->searchState = SE_IN;
//     QCursor cursor = this->cursor();
//     this->setCursor(Qt::BusyCursor);
//     //this->lastKeyword = this->ui_win.comboBox_2->currentText().trimmed();
//     if(this->lastKeyword.isEmpty()) {
//         //restore all item
//     }else{
//         //put keyword into completer object
//         this->kwAddWord(this->lastKeyword);
//         // if(this->ui_win.comboBox_2->completer() != this->kwCompleter) {
//         //     this->ui_win.comboBox_2->setCompleter(this->kwCompleter);
//         // }
//     }

// }
// void SNSNotify::clearSearch()
// {
//     if(this->searchState == SE_NON) {
//         return;
//     }

//     if(this->searchState == SE_IN) {
//         //this->statBar->showMessage(tr("We are searching, waiting..."));
//         this->showStatus(tr("We are searching, waiting..."));
//         return;
//     }
    
//     //this->ui_win.comboBox_2->clearEditText();
//     this->searchFeed();
//     this->searchState = SE_NON;
//     this->updateFeedStat(-1);
// }


void SNSNotify::showMessageHint(QString title, QString msg)
{
    if(this->msgHint == NULL) {
        msgHint = new MessageHint();        
        msgHint->setVisible(false);
    }
    msgHint->show(msg);
}
void SNSNotify::showStatus(QString str)
{
    this->statBar->showMessage(str, 10*1000);             
}

// void SNSNotify::initKWCompleter()
// {
//     if(this->kwCompleter == NULL) {
//         this->kwCompleter = new QCompleter(this);
//         this->kwCompleter->setCompletionMode(QCompleter::PopupCompletion);
//         const char *pn = "cvl";
//         QStringListModel * slm = new QStringListModel(this->kwCompleter);
//         this->kwCompleter->setProperty(pn, (uint)slm);
//         this->kwCompleter->setModel(slm);
//         //this->ui_win.comboBox_2->setModel(slm);
//     }    
// }
// void SNSNotify::kwAddWord(QString word)
// {
//     this->initKWCompleter();
//     const char *pn = "cvl";
//     QStringListModel * slm = (QStringListModel*)this->kwCompleter->property(pn).toUInt();    
//     Q_CHECK_PTR(slm);
//     QStringList sl = slm->stringList();
//     if(!sl.contains(word)) {
//         sl.append(word);
//         slm->setStringList(sl);
//     }
// }
// void SNSNotify::kwClearWord()
// {
//     this->initKWCompleter();
//     const char *pn = "cvl";
//     QStringListModel * slm = (QStringListModel*)this->kwCompleter->property(pn).toUInt();    
//     Q_CHECK_PTR(slm);
//     slm->setStringList(QStringList());
// }

void SNSNotify::test_live_update()
{
    this->updater = new LiveUpdate(this);

    updater->start();
}

////////////
///dlopen symbol
/////////////
#ifdef Q_WS_WIN
#define MY_EXPORT __declspec(dllexport)
#else
#define MY_EXPORT
#endif

extern "C" MY_EXPORT SNSNotify * get_sns_gui()
{
    q_debug()<<"here";
    so = new SystemOption(0);
    q_debug()<<"here";    
    SNSNotify * note = new SNSNotify();
    q_debug()<<"here";
    return note;
}

extern "C" MY_EXPORT void free_sns_gui(SNSNotify * uio)
{
    q_debug()<<"here";
    delete uio;
    q_debug()<<"here";
}

