
#include "utils.h"
#include "systemoption.h"
#include "useroption.h"
#include "sessionmanager.h"

#include "login.h"

Login::Login(QWidget *parent)
    : QWidget(parent)
{
    this->ui_win.setupUi(this);
    this->sessLoaded = 0;
    this->ui_win.comboBox_2->setDuplicatesEnabled(false);

    QObject::connect(this->ui_win.comboBox_2, SIGNAL(currentIndexChanged(int)),
                     this, SLOT(onUserNameSelectChanged(int)));
    QObject::connect(this->ui_win.pushButton_4, SIGNAL(clicked()), this, SLOT(login()));
    QObject::connect(this, SIGNAL(wantLogin(QString, QString, QList<int>)),
                     users, SLOT(login(QString, QString,  QList<int>)));

    //隐藏暂时不用的按钮，不影响现有代码的运行。
    // this->ui_win.checkBox_6->setVisible(false);
    // this->ui_win.checkBox_7->setVisible(false);
    // this->ui_win.checkBox_8->setVisible(false);
    // this->ui_win.checkBox_10->setVisible(false);
    //this->ui_win.label->setPixmap(QPixmap(so->iconPath+QString("/MISC_Social_Network_Circle_lg.jpg")).scaled(120,120));
    //6a00d8341ca27b53ef00e54f3b9c448834-800wi.gif
    this->ui_win.label->setPixmap(QPixmap(so->iconPath+QString("/applications-internet.png")));
}

Login::~Login()
{
}
void Login::showStatus(QString str)
{
    this->ui_win.label_2->setText(str);
}
void Login::login()
{
    QString userName = this->ui_win.comboBox_2->currentText();
    QString password = this->ui_win.lineEdit_2->text();
    QList<int> siteIds;

    if(!userName.trimmed().isEmpty() && !password.trimmed().isEmpty()) {

        qDebug()<<"goon log in";
        // if(this->ui_win.checkBox_6->isChecked()) {
        //     //siteIds |= SiteConst::SITE_HAINEI;
        //     siteIds.append(SiteConst::SITE_HAINEI);
        // }
        // if(this->ui_win.checkBox_7->isChecked()) {
        //     //siteIds |= SiteConst::SITE_XIAONEI;
        //     siteIds.append(SiteConst::SITE_XIAONEI);
        // }
        // if(this->ui_win.checkBox_8->isChecked()) {
        //     //siteIds |= SiteConst::SITE_KAIXIN;
        //     siteIds.append(SiteConst::SITE_KAIXIN);
        // }
        if(this->ui_win.checkBox_9->isChecked()) {
            siteIds.append(SiteConst::SITE_UCHOME_MAIN);
        }
        // if(this->ui_win.checkBox_10->isChecked()) {
        //     siteIds.append(SiteConst::SITE_UCHOME_MANYOU);
        // }
        
        q_debug()<<"all site should login:"<<siteIds;
        if(siteIds.count() == 0) {
            QString  msg = tr("You should select which site to login");
            //this->statBar->showMessage(msg);         
            this->showStatus(msg);
        }else{
            this->ui_win.pushButton_4->setEnabled(false);
            emit wantLogin(userName, password, siteIds);
            SessionManager sm;
            sm.addUser(userName, password);            
        }
    }else{
        //this->statBar->showMessage(tr("User name or password is empty"));
        this->showStatus(tr("User name or password is empty"));
    }

    //the right code is
    return ;    
}

void Login::showEvent ( QShowEvent * event )
{
    if(this->isVisible()) {
        if(this->sessLoaded == 0) {
            this->sessLoaded = 1;
            SessionManager sm;
            QStringList nameLists = sm.getAllNames();
            for(int i = 0 ; i < nameLists.count(); i ++) {
                this->ui_win.comboBox_2->insertItem(1, nameLists.at(i));
            }
            //q_debug()<<"load session done.";
        }
    }
    QWidget::showEvent(event);
}

void Login::onUserNameSelectChanged(int idx)
{
    QString username;
    QString password;
    SessionManager sm;

    username = this->ui_win.comboBox_2->currentText();
    password = sm.getUserPassword(username);
    this->ui_win.lineEdit_2->setText(password);
}

void Login::showLoginMessage(QString msg)
{
    this->showStatus(msg);
}

void Login::loginFaild(int status, QString reason)
{
    this->ui_win.pushButton_4->setEnabled(true);
    this->showLoginMessage(QString(tr("login error: ")) + reason);
}

