#include <stdio.h>
#include <stdlib.h>

#include <QApplication>

#include "utils.h"
#include "cookieparser.h"
#include "useroption.h"
#include "systemoption.h"

///////////////
SystemOption * so = NULL;//create instance at main.cpp after QApplication instanced

//////////////////////
/////
//////////////////////
SystemOption::SystemOption(QObject *parent)
    : QObject(parent)
{

    // SiteConst *sch = new SiteConst(this);
    // sch->siteId = SiteConst::SITE_HAINEI;
    // this->loadSymbol("hainei_feed", sch);

    // sch->siteName = tr("hainei");
    // sch->siteEncoding = "UTF-8";
    // sch->siteIcon = "hainei/favicon.ico";
    // sch->codec = QTextCodec::codecForName(sch->siteEncoding.toAscii());
    // Q_ASSERT(sch->codec != NULL);
    // sch->homePage = "http://www.hainei.com/";
    // sch->loginUrl = "/account?a=login&h=58e41d36";
    // sch->loginUserFieldName = "email";
    // sch->loginPasswordFieldName = "password";    
    // sch->loginAutoLoginFieldName = "autologin";
    // sch->loginAutoLoginFieldValue = "on";    
    // sch->parseLogin = &SiteConst::parseHaineiLoginResponse;
    // sch->errorMessageBegin = "<div class=\"msg-error\">";
    // sch->errorMessageEnd = "</div>";
    // sch->feedUrl = "/home";
    // sch->parseFetchFeed = &SiteConst::parseHaineiFetchFeedResponse;
    // sch->feedContentBegin = "<div class=\"sect-content\">";
    // sch->feedContentEnd = "<div class=\"feed-foot\" style=\"text-align:right\">";

    // //sch->letterUrls.insert(tr("inbox"), QPair<QString, void*>("/pmsgs", 0));
    // //sch->letterUrls.insert(tr("notice"), QPair<QString, void*>("/notification", 0));
    // sch->letterUrls.insert(SiteConst::LET_INBOX, 
    //                        QPair<QString, SC_LETTER_PARSER_TYPE>("/pmsgs", 
    //                                                              (&SiteConst::parseHaineiFetchLetterResponse)));

    // sch->letterUrls.insert(SiteConst::LET_NOTICE, 
    //                        QPair<QString, SC_LETTER_PARSER_TYPE>("/notification", 
    //                                                              (&SiteConst::parseHaineiNoticeFetchLetterResponse)));
    

    // SiteConst *scx = new SiteConst(this);
    // scx->siteId = SiteConst::SITE_XIAONEI;

    // scx->siteName = tr("xiaonei");
    // scx->siteEncoding = "UTF-8";
    // scx->siteIcon = "xiaonei/favicon.ico";
    // scx->codec = QTextCodec::codecForName(scx->siteEncoding.toAscii());
    // Q_ASSERT(scx->codec != NULL);
    // scx->homePage = "http://www.xiaonei.com/";
    // scx->loginUrl = "http://login.xiaonei.com/Login.do";
    // scx->loginUserFieldName = "email";
    // scx->loginPasswordFieldName = "password";
    // scx->loginAutoLoginFieldName = "autoLogin";
    // scx->loginAutoLoginFieldValue = "true";
    // scx->loginOtherValues.insert("origURL", "");    
    // scx->loginOtherValues.insert("formName", "");
    // scx->loginOtherValues.insert("method", "");
    // scx->parseLogin = &SiteConst::parseXiaoneiLoginResponse;
    // scx->errorMessageBegin = "<div class=\"errors_div\"><p>";
    // scx->errorMessageEnd = "</p></div>";
    // scx->feedUrl = "http://home.xiaonei.com/Home.do";
    // //http://www.xiaonei.com/stories.do?f=-1
    // //"http://guide.xiaonei.com/guide.do";
    // scx->parseFetchFeed = &SiteConst::parseXiaoneiFetchFeedResponse;
    // scx->feedContentBegin = "feedHome";
    // scx->feedContentEnd = "<div class=\"more-feed\">";

    // //scx->letterUrls.insert(tr("inbox"), QPair<QString, void*>("http://msg.xiaonei.com/message/inbox.do", 0));
    // //scx->letterUrls.insert(tr("notice"), QPair<QString, void*>("http://msg.xiaonei.com/notify/notifications.do", 0));
    // scx->letterUrls.insert(SiteConst::LET_INBOX, 
    //                        QPair<QString, SC_LETTER_PARSER_TYPE>("http://msg.xiaonei.com/message/inbox.do", 
    //                                                              (&SiteConst::parseXiaoneiFetchLetterResponse)));
    // // scx->letterUrls.insert(SiteConst::LET_NOTICE, 
    // //                        QPair<QString, SC_LETTER_PARSER_TYPE>("http://msg.xiaonei.com/notify/notifications.do", 
    // //                                              (&SiteConst::parseUnimpledFetchLetterResponse)));
    

    // SiteConst *sck = new SiteConst(this);
    // sck->siteId = SiteConst::SITE_KAIXIN;

    // sck->siteName = tr("kaixin");
    // sck->siteEncoding = "UTF-8";
    // sck->siteIcon = "kaixin/kaixinlogo1.gif";
    // sck->codec = QTextCodec::codecForName(sck->siteEncoding.toAscii());
    // Q_ASSERT(sck->codec != NULL);
    // sck->homePage = "http://www.kaixin001.com/";
    // sck->loginUrl = "/login/login.php";
    // sck->loginUserFieldName = "email";
    // sck->loginPasswordFieldName = "password";
    // sck->loginAutoLoginFieldName = "remember";
    // sck->loginAutoLoginFieldValue = "1";
    // sck->loginOtherValues.clear();
    // sck->loginOtherValues.insert("url", "/");
    // sck->loginOtherValues.insert("invisible_mode", "0");
    // sck->parseLogin = &SiteConst::parseKaixinLoginResponse;
    // sck->errorMessageBegin = "var obj = $(\"prompt\" + \"";
    // sck->errorMessageEnd = "\");";
    // //see doc/kaixin_login_error.html for error msg search method
    // sck->feedUrl = "/home/news.php";
    // sck->parseFetchFeed = &SiteConst::parseKaixinFetchFeedResponse;
    // sck->feedContentBegin = "newslistdiv";
    // sck->feedContentEnd = "<script language=javascript>";

    // // sck->letterUrls.insert(tr("inbox"), QPair<QString, void*>("/msg/inbox.php", 0));
    // // sck->letterUrls.insert(tr("pubmsg"), QPair<QString, void*>("/msg/sys.php?t=15", 0));
    // // sck->letterUrls.insert(tr("comment"), QPair<QString, void*>("/comment/index.php?t=73", 0));
    // // sck->letterUrls.insert(tr("commentreply"), QPair<QString, void*>("/comment/send.php?t=98", 0));
    // // sck->letterUrls.insert(tr("bbs"), QPair<QString, void*>("/comment/uindex.php?t=75", 0));
    // // sck->letterUrls.insert(tr("bbsreply"), QPair<QString, void*>("/comment/usend.php?t=56", 0));
    // sck->letterUrls.insert(SiteConst::LET_INBOX, 
    //                        QPair<QString, SC_LETTER_PARSER_TYPE>("/msg/inbox.php", 
    //                                                              (&SiteConst::parseKaixinFetchLetterResponse)));
    // sck->letterUrls.insert(SiteConst::LET_PUBMSG, 
    //                        QPair<QString, SC_LETTER_PARSER_TYPE>("/msg/sys.php?t=15", (&SiteConst::parseKaixinFetchPubMsgLetterResponse)));
    // // sck->letterUrls.insert(SiteConst::LET_COMMENT, QPair<QString, void*>("/comment/index.php?t=73", 0));
    // // sck->letterUrls.insert(SiteConst::LET_COMMENT_REPLY, QPair<QString, void*>("/comment/send.php?t=98", 0));
    // // sck->letterUrls.insert(SiteConst::LET_BBS, QPair<QString, void*>("/comment/uindex.php?t=75", 0));
    // // sck->letterUrls.insert(SiteConst::LET_BBS_REPLY, QPair<QString, void*>("/comment/usend.php?t=56", 0));


    // SiteConst *scu = new SiteConst(this);
    // scu->siteId = SiteConst::SITE_UCHOME;

    // scu->siteName = tr("uchome");
    // scu->siteEncoding = "GBK";
    // scu->siteIcon = "uchome/favicon.ico";
    // scu->codec = QTextCodec::codecForName(scu->siteEncoding.toAscii());
    // Q_ASSERT(sck->codec != NULL);
    // scu->homePage = "http://u.discuz.net/home/";
    // scu->loginUrl = "/do.php?ac=login";
    // scu->loginUserFieldName = "username";
    // scu->loginPasswordFieldName = "password";
    // scu->loginAutoLoginFieldName = "cookietime";
    // scu->loginAutoLoginFieldValue = "315360000";
    // scu->loginOtherValues.clear();
    // scu->loginOtherValues.insert("seccode", "");
    // scu->loginOtherValues.insert("loginsubmit", QUrl::fromPercentEncoding("%B5%C7%C2%BC"));
    // scu->parseLogin = &SiteConst::parseUCHomeLoginResponse;
    // //need change error msg beign end;
    // scu->errorMessageBegin = "var obj = $(\"prompt\" + \"";
    // scu->errorMessageEnd = "\");";
    // scu->feedUrl = "/space.php?do=home&view=we";//"/space-home-view-we.html";
    // scu->parseFetchFeed = &SiteConst::parseUCHomeFetchFeedResponse;
    // scu->feedContentBegin =  "feed_div";
    // scu->feedContentEnd = "<div class=\"page\">";
    // scu->hasCheckCode = true; 
    // //未读消息 收件箱 发件箱 公共消息  通知 应用消息                 
    // // scu->letterUrls.insert(tr("newbox"), QPair<QString, void*>("/space.php?do=pm&view=newbox", 0));
    // // scu->letterUrls.insert(tr("inbox"), QPair<QString, void*>("/space.php?do=pm&view=inbox", 0));
    // // scu->letterUrls.insert(tr("pubmsg"), QPair<QString, void*>("/space.php?do=pm&view=announce", 0));
    // // scu->letterUrls.insert(tr("notice"), QPair<QString, void*>("/space.php?do=notice", 0));
    // // scu->letterUrls.insert(tr("appmsg"), QPair<QString, void*>("/space.php?do=notice&view=userapp", 0));

    // // scu->letterUrls.insert(SiteConst::LET_NEWBOX, QPair<QString, void*>("/space.php?do=pm&view=newbox", 0));
    // scu->letterUrls.insert(SiteConst::LET_INBOX, 
    //                        QPair<QString, SC_LETTER_PARSER_TYPE>("/space.php?do=pm&view=inbox", 
    //                                                              (&SiteConst::parseUCHomeFetchLetterResponse)));
    // scu->letterUrls.insert(SiteConst::LET_PUBMSG, 
    //                        QPair<QString, SC_LETTER_PARSER_TYPE>("/space.php?do=pm&view=announce", 
    //                                                              (&SiteConst::parseUCHomeFetchLetterResponse)));
    // scu->letterUrls.insert(SiteConst::LET_NOTICE, 
    //                        QPair<QString, SC_LETTER_PARSER_TYPE>("/space.php?do=notice", 
    //                                                              (&SiteConst::parseUCHomeNoticeFetchLetterResponse)));
    // // scu->letterUrls.insert(SiteConst::LET_APPMSG, QPair<QString, void*>("/space.php?do=notice&view=userapp", 0));


    // SiteConst *scm = scu->dup();

    // scm->homePage = "http://uchome.developer.manyou.com/uchome/";
    // scm->siteName = QUrl(scm->homePage).host();
    // scm->hasCheckCode = false;
    // scm->loginOtherValues.insert("formhash", "3040598c");

    // this->siteList.append(sch);
    // this->siteList.append(scx);
    // this->siteList.append(sck);
    // this->siteList.append(scu);
    // this->siteList.append(scm);

    this->updateIntval = 1*60;//10*60; //10 minutes
    this->historyLifeTime = 76*3600; //76 hours
    this->numOnePage = 12;
    //
    for(int i = BS_MIN + 1 ; i < BS_MAX ; i ++) {
        this->browserList<<i;
    }

    //
    this->codec = QTextCodec::codecForName("UTF-8");    
    Q_ASSERT(this->codec != NULL);

    //path config
    appPath = qApp->applicationDirPath();
    this->iconPath = appPath + "/icons/";
#ifdef Q_OS_WIN
    this->dotPath = appPath + "/.snsnotify/";
#else
    this->dotPath = QDir::homePath() + "/.snsnotify/";
#endif
    this->cachePath = this->dotPath + "/cache/";
    if(!QDir().exists(this->dotPath)) QDir().mkdir(this->dotPath);
    if(!QDir().exists(this->cachePath)) QDir().mkdir(this->cachePath);
    this->modulePath = this->appPath + "/modules/";

    this->modules.insert(SiteConst::SITE_UCHOME_MAIN, QString("uch_main_feed"));
}

SystemOption::~SystemOption()
{
    
}

bool SystemOption::loadSymbol(const char *name, SiteConst **sc)
{
    Q_ASSERT(name != NULL);

    QString module_path;
    QLibrary *so_handle = NULL;
    SiteConst *(*get_site_const)(int) = NULL;

    module_path = so->appPath + "/modules/" + Utils::get_so_file_name(name);
    q_debug()<<"Load "<<module_path;
    so_handle = new QLibrary(module_path);
    if(!so_handle->load()) {
        Q_ASSERT(1 == 2);
    }
    *(void**)(&get_site_const) = so_handle->resolve("get_site_const");
    Q_ASSERT(get_site_const != NULL);
    *sc = get_site_const(0);

    q_debug()<<"modver:"<<((*sc)->get_version())<<(*sc);

    return true;
}

SiteConst *SystemOption::getSite(int id)
{
    Q_ASSERT(id > SiteConst::SITE_MIN && id < SiteConst::SITE_MAX);

    SiteConst *sc = NULL;
    if(this->siteMap.contains(id)) {
        sc = this->siteMap[id];
    }else{
        if(this->modules.contains(id)) {
            if(!loadSymbol(this->modules[id].toAscii().data(), &sc)) {
                sc = NULL;
                Q_ASSERT(sc != NULL);
            }
        }else{
            return NULL;
        }
    }
    return sc;
}

