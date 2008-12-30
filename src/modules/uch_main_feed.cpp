#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>

#include <QtCore>
#include <QtGui>
#include <QtWebKit>
#include <QtNetwork>

#include "md5.h"
#include <tidy.h>
#include "tidy_wrapper.h"
#include "feed_record.h"
#include "sns_feed.h"
#include "utils.h"

#include "uch_main_feed.h"
/*
  TidyNode node;
  for(node = first_feed(doc); node; node = next_feed(doc, node))
  {
  const char * feed_text = parse_feed(doc, node);
  }  
*/

class ParserStateObject
{
public:
    ParserStateObject(){}
    ~ParserStateObject(){}

    QString curr_date;  // for feed
    int  curr_index; //for note
    int  total_page_count;// for friends
    int  curr_page_index;//
    int  total_friend_count;
    int  curr_friend_index;//
};

/////////impl
UCHome_Main_SiteConst::UCHome_Main_SiteConst()
{
    site_name = "uchome";
    site_icon = "faviocn.ico";
    home_page = "http://u.discuz.net/home/";
    encoding = "gbk";
    host = "u.discuz.net";
    port = 80;
    login_url = "/home/do.php?ac=login&&ref";
    username_field_name="username";
    password_field_name="password";
    data = QString::null;
    cookies = QString::null;
    username = QString::null;
    password = QString::null;
    feed_url = "http://u.discuz.net/home/space.php?do=home&view=we";
    other_fields.append(QPair<QString,QString>("cookietime", "315360000"));
    step = 0;
    next = 0;  // 0, 1
    //char **letter_urls;
    letter_urls.append("");
    //int  (*letter_parsers[10])();    

    friend_url = "http://u.discuz.net/home/cp.php?ac=friend&op=group";

    codec = QTextCodec::codecForName(encoding.toAscii().data());

    /////////ctrl
    this->isLoging.resize(1);
    this->isLoging.setBit(0, false);
    this->loginRetryTimes = 3;
}
UCHome_Main_SiteConst::~UCHome_Main_SiteConst()
{
}

int UCHome_Main_SiteConst::get_version()
{
    return 123;
}
//q 必须是UTF-8编码的问题字符串
int   UCHome_Main_SiteConst::uch_question_caculor(QString q)
{
    int r = 0;
    QString tmpstr;
    QString u8qstr = q;
    QRegExp rx("(\\d+)(\\D+)(\\d+)(\\D+)");
    int op1, op2;
    QString opmd5;

    rx.setMinimal(true);
    rx.exactMatch(u8qstr);
    for(int i = 0 ; i < rx.numCaptures(); i++) {
        tmpstr = this->u8codec->toUnicode(rx.cap(i).toAscii());
        q_debug()<<i<<rx.cap(i)<<tmpstr;
                 // <<(rx.cap(i) == QString("减"))
                 // <<(rx.cap(i) == QString("加"))
                 // <<(rx.cap(i) == QString("乘"))
                 // <<(rx.cap(i) == QString("除"))
            ;
    }
    Q_ASSERT(rx.numCaptures() > 3);
    op1 = rx.cap(1).toInt();
    op2 = rx.cap(3).toInt();
    opmd5 = this->md5(rx.cap(2).toAscii().data());
    if(opmd5 == QString("85be08f9260a8c930f333201afe6c54d")) {// == QString("加")) {
        r = op1 + op2;
    }else if(opmd5 == QString("41f5f6ef4357e87643cc72b113fe5c78")) { //QString("减")) {
        r = op1 - op2;
    }else if(opmd5 == QString("f3a668c23d06093727b412771cf927f2")) { //QString("乘")) {
        r = op1 * op2;
    }else if(opmd5 == QString("9fdac49d5e4665e43ac913b86339a2a2")) { //QString("除")) {
        r = op1 / op2;
    }else{
        q_debug()<<"No impled OP";
        Q_ASSERT(1 == 2);
    }

    // QString addmd5 = this->md5(QString("加").toAscii().data());
    // QString submd5 = this->md5(QString("减").toAscii().data());
    // QString chengmd5 = this->md5(QString("乘").toAscii().data());
    // QString divmd5 = this->md5(QString("除").toAscii().data());

    //q_debug()<<"加:"<<addmd5; // 85be08f9260a8c930f333201afe6c54d
    //q_debug()<<"减:"<<submd5;  // 41f5f6ef4357e87643cc72b113fe5c78
    //q_debug()<<"乘:"<<chengmd5; // f3a668c23d06093727b412771cf927f2
    //q_debug()<<"除:"<<divmd5;  // 9fdac49d5e4665e43ac913b86339a2a2

    return r;
}
int UCHome_Main_SiteConst::login(Account *acc)
{
    QByteArray login_step1_html, login_step2_html, login_step3_html, login_seccode_html;
    QByteArray login_step1_u8html, login_step2_u8html, login_step3_u8html, login_seccode_u8html;
    int rc = -1;
    TidyDoc tdoc;
    TidyNode node1;
    TidyNode node2; 
    TidyNode node3;
    TidyAttr attr;
    ctmbstr  str;
    ctmbstr formhash;
    QByteArray formhashbytes;
    TidyBuffer tbuf;
    QByteArray question;
    QByteArray answer;
    QString orig_action;

    emit this->showLoginMessage(tr("Init login emulate enviriant."));
    this->isLoging.setBit(0, true);

    this->webpage = new QWebPage();
    this->nam = this->webpage->networkAccessManager();
    this->webframe = this->webpage->mainFrame();
    QObject::connect(this->webpage, SIGNAL(loadStarted()),
                     this, SLOT(wkLoadStarted()));

    QObject::connect(this->webpage, SIGNAL(loadFinished(bool)),
                     this, SLOT(wkLoadFinished(bool)));

    QObject::connect(this->webpage, SIGNAL(loadProgress(int)),
                     this, SLOT(wkLoadProgress(int)));
    //step 1
    //this->webview->setUrl(this->home_page);
    q_debug()<<"Loading: "<<this->home_page;
    this->webpage->mainFrame()->setUrl(this->home_page);
    //this->webpage->mainFrame()->load(QUrl("http://localhost"));
    q_debug()<<this->webpage<<qApp;
    
    return FETCH_FEED_ERROR;

    //////////next step4
    QTcpSocket *sock = NULL;
    
    sock = new QTcpSocket();
    sock->connectToHost(this->host, this->port);
    if(!sock->waitForConnected()) {
        q_debug()<<"error:";
        return LOGIN_ERROR;
    }

    QString data ;
    data = QString("username=%1&password=%2&seccode=%3&formhash=%4&cookietime=315360000").arg(this->username, this->password, QString(answer), QString(formhashbytes)) ;
    data += "&refer=space.php%3Fdo%3Dhome&loginsubmit=%B5%C7%C2%BC";
    
    QString request;
    request = 
        QString("POST %1 HTTP/1.1\r\n"
                "Host: %2\r\n"
                "User-Agent: Mozilla/5.0 (X11; U; Linux i686; zh-CN; rv:1.9.0.4) Gecko/2008112913 Gentoo Minefield/3.0.4\r\n"
                "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
                "Accept-Language: zh-cn,zh;q=0.5\r\n"
                "Accept-Charset: gb2312,utf-8;q=0.7,*;q=0.7\r\n"
                "Keep-Alive: 300\r\n"
                "Connection: keep-alive\r\n"
                "Referer: %3\r\n"
                "Cookie: uchome_seccode=0\r\n"
                "Content-Type: application/x-www-form-urlencoded\r\n"
                "Content-Length: %4\r\n\r\n")
        .arg(this->login_url, this->host, this->home_page, 
             QString("%1").arg(data.length()));

    ;
    request += data;
    q_debug()<<request;

    sock->write(request.toAscii());
    if(!sock->waitForBytesWritten()) {
        q_debug()<<"write error";
        return LOGIN_ERROR;
    }
    char header[8192] = {0} ;
    char b2[2] = {0};
    char *bp = &header[0];
    char *tp = bp;
    qint64 rlen = 0;
    qint64 oklen = 0;
    while(true) {
        if(!sock->waitForReadyRead()) {
            q_debug()<<"wait read error";
            return LOGIN_ERROR;
        }
        oklen = sock->bytesAvailable();
        if(oklen < 0 ) {
            q_debug()<<"wait read errorsdfjsdifsdifsdifj";
        }
        while(oklen-- > 0) {
            rlen = sock->read(b2, 1);
            *tp++ = b2[0];
            if(tp - bp > 4 && strncmp(tp-4, "\r\n\r\n", 4) == 0) {
                break;
            }
        }
        if(strncmp(tp-4, "\r\n\r\n", 4) == 0) {
            break;
        }
    }
    //parse header, got file length
    QStringList cookie_lines;
    QStringList header_lines = QString(header).split("\r\n");
    qint64 content_length = -1;
    for(int i = 0; i < header_lines.count(); i ++) {
        if(header_lines.at(i).startsWith("Content-length")) {
            content_length = header_lines.at(i).split(":").at(1).trimmed().toLongLong();
        }
        if(header_lines.at(i).startsWith("Set-Cookie:")
           && header_lines.at(i).indexOf("=deleted") == -1) {
            cookie_lines<<header_lines.at(i);
        }
    }
    q_debug()<<"Got Content-length:"<<content_length<<"\n"<<header
             <<cookie_lines;
    if(cookie_lines.count() == 0) {
        q_debug()<<"No login ok cookie found";
        return LOGIN_ERROR;
    }
    int has_uchome_auth = 0;
    int has_uchome_loginuser = 0;
    for(int i = 0 ; i < cookie_lines.count(); i ++) {
        if(cookie_lines.at(i).indexOf("uchome_auth=") != -1) {
            has_uchome_auth = 1;
        }
        if(cookie_lines.at(i).indexOf("uchome_loginuser=") != -1) {
            has_uchome_loginuser = 1;
        }        
    }
    if(!has_uchome_auth || !has_uchome_loginuser) {
        q_debug()<<"may be login faild";
        return LOGIN_ERROR;
    }
    this->combine_cookies(cookie_lines);
    return LOGIN_OK;

    //no use code
    if(content_length == -1) {
        q_debug()<<"No content-length field";
        //return -1;
    }
    
    QString html ;

    while(true) {
        if(!sock->waitForReadyRead()) {
            q_debug()<<"wait read error";
            return -1;
        }
        oklen = sock->bytesAvailable();
        if(oklen > 0) {
            html += sock->readAll();
        }else{
            break;
        }
    }

    q_debug()<<"Got conent size:"<<html.length()
             <<"\n"<<html;

    return 0;    
}
int UCHome_Main_SiteConst::logout()
{
    return 0;
}
int UCHome_Main_SiteConst::fetchFeedData() 
{
    q_debug()<<"impled";
    
    QTcpSocket *sock = NULL;

    sock = new QTcpSocket();
    
    sock->connectToHost(this->host, this->port);
    if(!sock->waitForConnected()) {
        q_debug()<<"Connect error";
        return FETCH_FEED_ERROR;
    }
    
    QString request;
    request = 
        QString(
                "GET %1 HTTP/1.1\r\n"
                "Host: uchome.developer.manyou.com\r\n"
                "User-Agent: Opera/9.62 (X11; Linux i686; U; zh-cn)\r\n"
                "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
                "Accept-Language: zh-cn,zh;q=0.5\r\n"
                "Accept-Charset: gb2312,utf-8;q=0.7,*;q=0.7\r\n"
                //"Keep-Alive: 300\r\n"
                //"Connection: keep-alive\r\n"
                "Connection: close\r\n"
                "Cookie: %2\r\n"
                "\r\n"
                )
        .arg(this->feed_url, this->cookies);

    q_debug()<<request;
    sock->write(request.toAscii());
    if(!sock->waitForBytesWritten()) {
        q_debug()<<"Write error";
        return FETCH_FEED_ERROR;
    }

    char header[8192] = {0} ;
    char b2[2] = {0};
    char *bp = &header[0];
    char *tp = bp;
    qint64 rlen = 0;
    qint64 oklen = 0;

    if(!sock->waitForReadyRead()) {
        q_debug()<<"wait read error";
        return LOGIN_ERROR;
    }

    while(true) {
        oklen = sock->bytesAvailable();
        if(oklen < 0 ) {
            q_debug()<<"wait read errorsdfjsdifsdifsdifj";
        }
        while(oklen-- > 0) {
            rlen = sock->read(b2, 1);
            *tp++ = b2[0];
            if(tp - bp > 4 && strncmp(tp-4, "\r\n\r\n", 4) == 0) {
                break;
            }
        }
        if(strncmp(tp-4, "\r\n\r\n", 4) == 0) {
            break;
        }
    }
    //parse header, got file length
    QString html_header = header;

    QStringList header_lines = QString(header).split("\r\n");
    qint64 content_length = -1;
    for(int i = 0; i < header_lines.count(); i ++) {
        if(header_lines.at(i).startsWith("Content-length")) {
            content_length = header_lines.at(i).split(":").at(1).trimmed().toLongLong();
        }
    }
    
    if(content_length == -1) {
        q_debug()<<"Unknown content-length for header:"<< html_header;
        //return FETCH_FEED_ERROR;
    }

    QByteArray html;
    QByteArray hb ;
    while(true) {
        oklen = sock->bytesAvailable();
        if(oklen > 0) {
            hb= sock->readAll();
            html += hb;
        }else if(oklen == 0) {
            if(!sock->waitForReadyRead()) {
                q_debug()<<"wait read error";
                break;
            }
        }else{
            q_debug()<<"bytesAvalibale < 0";
            break;
        }
    }
    
    this->feed_page_html = html;
    q_debug()<<"read len =? content-length: "<<html.length()<<" =? "<<content_length;
    q_debug()<<html.left(20)<<html.right(20);
    this->clean_my_name_lable();
    q_debug()<<this->feed_page_html.left(20)<<this->feed_page_html.right(20);
    //q_debug()<<this->feed_page_html;

    // QFile fp("hehe.html");
    // fp.open(QIODevice::ReadWrite);
    // fp.write(this->feed_page_html);
    // fp.close();

    return FETCH_FEED_OK;
}
TidyNode UCHome_Main_SiteConst::first_feed(void *state_data, TidyDoc doc)
{
    ParserStateObject *state_obj = (ParserStateObject*)state_data;
    TidyNode node = NULL;
    TidyNode node2 = NULL;

    ////////
    this->signtext = this->find_sign_text(doc);    
    this->photourl = this->find_photo_url(doc);
    this->activetext = this->find_active_text(doc);
    this->userid = this->parse_userid(doc);
    //this->sysnotes = this->find_sys_notice(doc);

    //////////////
    node = this->searchNode(doc, NULL, "enter-content", TidyTag_DIV);
    if(node == NULL) {
        q_debug()<<"No feed node found";
        return NULL;
    }
    node2 = tidyGetChild(node);
    assert(node2 != NULL);
    int nid = tidyNodeGetId(node2);
    if(nid == TidyTag_H4) {
        state_obj->curr_date = this->get_time_string(doc, node2);
        node = tidyGetNext(node2);
        if(node == NULL) {
            //null
        }else{
            return tidyGetChild(node);
        }
    }else if(nid == TidyTag_UL) {
        node = tidyGetChild(node2);
        if(tidyNodeGetId(node) == TidyTag_LI) {
            return node; // okkkkkk
        }
    }else{
        q_debug()<<"Unknown node type:"<<tidyNodeGetName(node2);
    }
    
    return NULL;
}

TidyNode UCHome_Main_SiteConst::next_feed(void *state_data, TidyDoc doc, TidyNode prev)
{
    ParserStateObject *state_obj = (ParserStateObject*)state_data;
    TidyNode node = NULL;
    TidyNode node2 = NULL;
    int nid = 0;
    
    node = tidyGetNext(prev);
    if(node == NULL) {
        node = tidyGetParent(prev);
        assert(node != NULL);
        node2 = tidyGetNext(node);
        if(node2 == NULL) {
            return NULL;
        }else{
            if(tidyNodeGetId(node2) == TidyTag_H4) {
                state_obj->curr_date = this->get_time_string(doc, node2);
                node = tidyGetNext(node2);
                if(node == NULL) {
                    //null
                }else{
                    return tidyGetChild(node);
                }
            }else{
                nid = tidyNodeGetId(node2);
                assert(nid == TidyTag_UL);
                return tidyGetChild(node2);
            }
        }
    }else{
        return node;
    }
    return NULL;
}

FeedRecord* UCHome_Main_SiteConst::parse_feed(void *state_data, TidyDoc doc, TidyNode node)
{
    FeedRecord *rec = NULL;
    QString feed_text ;
    QByteArray feed_bytes;
    int nid = 0;
    ctmbstr  fid = NULL;
    TidyAttr attr = NULL;
    TidyBuffer tbuf = {0};
    tidyBufInit(&tbuf);
    
    nid = tidyNodeGetId(node);    
    //this->get_node_text(doc, node, &tbuf);
    //feed_bytes = QByteArray((char*)tbuf.bp);
    //feed_text = this->u8codec->toUnicode(feed_bytes);
    //tidyBufFree(&tbuf);
    feed_bytes = this->save_node_with_absolute_link_and_filter(doc, node, TidyTag_A, "float_cancel");
    feed_text = this->u8codec->toUnicode(feed_bytes);    

    rec = new FeedRecord();
    rec->content = feed_text;

    //<li class="type_1006780" id="feed_685697_li">
    //这个id是唯一的，用这个做标识
    attr = tidyAttrGetById(node, TidyAttr_ID);
    fid = tidyAttrValue(attr);
    //q_debug()<<attr<<fid;
    if(fid == NULL) {
        q_debug()<<"Warning: invalid fid"<<fid<<feed_text;
        return NULL;
    }else if(strstr(fid, " 1000 ") != NULL) {
        feed_bytes = fid;
        feed_bytes = feed_bytes.replace(" 1000 ", "");
        fid = feed_bytes.data();
        q_debug()<<"Warning: invalid fid"<<tidyAttrValue(attr);
        md5CheckSum(fid, strlen(fid), rec->md5sum);
    }else{
        md5CheckSum(fid, strlen(fid), rec->md5sum);
    }
    return rec;
}

void * UCHome_Main_SiteConst::create_parser_state_data()
{
    ParserStateObject *so = new ParserStateObject();

    return so;
}
int UCHome_Main_SiteConst::free_parser_state_data(void *data)
{
    Q_ASSERT(data != NULL);
    ParserStateObject *so = (ParserStateObject*)data;

    delete so;
    return SNS_OK;
}

void UCHome_Main_SiteConst::clean_my_name_lable()
{
    int n = 0;
    char ch ;
    int pos = this->feed_page_html.indexOf("<");
    this->feed_page_html = this->feed_page_html.right(this->feed_page_html.length()-pos);
    pos = this->feed_page_html.lastIndexOf(">");
    this->feed_page_html = this->feed_page_html.left(pos+1);
    QByteArray tarr = this->feed_page_html;

    this->feed_page_html = this->feed_page_html.replace("\r\n1000\r\n", "");
    this->feed_page_html = this->feed_page_html.replace("\r\n2000\r\n", "");
    this->feed_page_html = this->feed_page_html.replace("\r\n3000\r\n", "");
    this->feed_page_html = this->feed_page_html.replace("\r\n4000\r\n", "");
    this->feed_page_html = this->feed_page_html.replace("\r\n5000\r\n", "");
    this->feed_page_html = this->feed_page_html.replace("\r\n6000\r\n", "");
    this->feed_page_html = this->feed_page_html.replace("\r\n7000\r\n", "");
    this->feed_page_html = this->feed_page_html.replace("\r\n8000\r\n", "");
    this->feed_page_html = this->feed_page_html.replace("\r\n9000\r\n", "");
    // pos = 0;
    // while(true) {
    //     pos = this->feed_page_html.indexOf("1000", pos);
    //     if(pos == -1) {
    //         break;
    //     }
    //     q_debug()<<this->feed_page_html.mid(pos - 10, 30);
    //     this->feed_page_html.remove(pos, 4);
    //     n = pos;
    //     while(true) {
    //         ch = this->feed_page_html.at(n);
    //         if(ch == ' ' || ch == '\r' || ch == '\n') {
    //             this->feed_page_html.remove(n,1);
    //             q_debug()<<"ch:'"<<ch<<"'";
    //         }else{
    //             break;
    //         }
    //     }
    //     n = pos - 1;
    //     while(true) {
    //         ch = this->feed_page_html.at(n);
    //         if(ch == ' ' || ch == '\r' || ch == '\n') {
    //             this->feed_page_html.remove(n,1);
    //             n--;
    //             q_debug()<<"ch:'"<<ch<<"'";
    //         }else{
    //             break;
    //         }
    //     }
    // }

    // pos = 0;
    // while(true) {
    //     pos = this->feed_page_html.indexOf("2000", pos);
    //     if(pos == -1) {
    //         break;
    //     }
    //     q_debug()<<this->feed_page_html.mid(pos - 10, 30);
    //     this->feed_page_html.remove(pos, 4);
    //     n = pos;
    //     while(true) {
    //         ch = this->feed_page_html.at(n);
    //         if(ch == ' ' || ch == '\r' || ch == '\n') {
    //             this->feed_page_html.remove(n,1);
    //             q_debug()<<"ch:'"<<ch<<"'";
    //         }else{
    //             break;
    //         }
    //     }
    //     n = pos - 1;
    //     while(true) {
    //         ch = this->feed_page_html.at(n);
    //         if(ch == ' ' || ch == '\r' || ch == '\n') {
    //             this->feed_page_html.remove(n,1);
    //             n--;
    //             q_debug()<<"ch:'"<<ch<<"'";
    //         }else{
    //             break;
    //         }
    //     }
    // }

    this->feed_page_utf8_html = this->u8codec->fromUnicode(this->codec->toUnicode(this->feed_page_html));
}

QString UCHome_Main_SiteConst::get_time_string(TidyDoc doc, TidyNode tnode)
{
    QString time_string ;

    TidyBuffer tbuf = {0};
    tidyBufInit(&tbuf);
    tidyNodeGetText(doc, tnode, &tbuf);
    if(strstr((char*)tbuf.bp, "2008-") != NULL) {                
        QDate onlyDate = QDate::fromString(QString((char*)tbuf.bp).trimmed(), "yyyy-MM-dd");
        QDateTime currDate = QDateTime::currentDateTime();
        currDate.setDate(onlyDate);
        //q_debug()<<currDate<<onlyDate<<((char*)tbuf.bp);
        time_string = onlyDate.toString();
    }else{
        //must yesterday
        QDateTime currDate = QDateTime::currentDateTime().addDays(-1);
        time_string = currDate.date().toString();
    }
    tidyBufFree(&tbuf);


    return time_string;
}
QString UCHome_Main_SiteConst::format_human_feed(FeedRecord *rec)
{
    Q_ASSERT(rec != NULL);
    QString formated_feed;

    formated_feed = rec->content;

    return formated_feed;
}

QString UCHome_Main_SiteConst::find_nick_name(TidyDoc doc)
{
    QString nickname;



    return nickname;
}
QString UCHome_Main_SiteConst::find_sign_text(TidyDoc doc)
{
    QString sign_text;
    TidyNode node;
    TidyNode node2;
    TidyNode node3;
    TidyBuffer tbuf;
    
    node = this->searchNode(doc, NULL, "mystate", TidyTag_DIV);
    if(node != NULL) {
        node2 = tidyGetChild(node);
        tidyBufInit(&tbuf);
        if(tidyNodeGetType(node2) == TidyNode_Text) {
            //no sign text set now;
            tidyNodeGetText(doc, (node2), &tbuf);
        }else{
            Q_ASSERT(tidyNodeGetId(node2) == TidyTag_A);
            tidyBufInit(&tbuf);
            node3 = tidyGetNext(node2);        
            if(tidyNodeGetText(doc, (node3), &tbuf)) {
            }
        }
        sign_text = this->u8codec->toUnicode(QByteArray((char*)tbuf.bp));
        sign_text = sign_text.trimmed();
        //drop this chars &nbsp;(
        sign_text = sign_text.left(sign_text.length() - 7);
        q_debug()<<"Sign text:"<<sign_text;
        tidyBufFree(&tbuf);
    }else{
        q_debug()<<"Warning: no state sign text found";
    }

    return sign_text;
}
QString UCHome_Main_SiteConst::find_photo_url(TidyDoc doc)
{
    QString photo_url;
    TidyNode node;
    TidyNode node2;
    TidyNode node3;
    ctmbstr url_str = NULL;
    TidyAttr  attr = NULL;
    
    //quick_update s_clear
    node = this->searchNode(doc, NULL, "composer_header", TidyTag_DIV);
    if(node != NULL) {
        node2 = tidyGetChild(node);
        node3 = this->searchNode(doc, node2, NULL, TidyTag_IMG);
        if(node3 != NULL) {
            attr = tidyAttrGetById(node3, TidyAttr_SRC);
            if(attr != NULL) {
                url_str = tidyAttrValue(attr);
                photo_url = QString(url_str);
                photo_url = photo_url.replace("small", "big");
                q_debug()<<"Photo url: "<<photo_url;
            }else{
                q_debug()<<"Warning: no photo url found";
            }            
        }else{
            q_debug()<<"Warning: no photo url found";
        }
    }else{
        q_debug()<<"Warning: no photo url found";
    }
    
    return photo_url;
}
QString UCHome_Main_SiteConst::find_active_text(TidyDoc doc)
{
    QString active_text;
    TidyNode node;
    TidyNode node2;
    TidyNode node3;
    ctmbstr url_str = NULL;
    TidyAttr  attr = NULL;
    TidyBuffer tbuf = {0};
    
    //quick_update s_clear
    node = this->searchNode(doc, NULL, "index_name", TidyTag_H3);
    if(node != NULL) {
        node2 = tidyGetNext(node);
        if(node2 != NULL && tidyNodeGetId(node2) == TidyTag_P) {
            tidyBufInit(&tbuf);
            this->get_node_text(doc, node2, &tbuf);
            q_debug()<<QByteArray((char*)tbuf.bp);
            active_text = this->u8codec->toUnicode(QByteArray((char*)tbuf.bp)).trimmed();
            tidyBufFree(&tbuf);
            q_debug()<<"active text: "<<active_text;
        }else{
            q_debug()<<"Warning: no active text found";
        }
    }else{
        q_debug()<<"Warning: no active text found";
    }
    
    return active_text;
    
}
QVector<QString> UCHome_Main_SiteConst::find_sys_notice(TidyDoc doc)
{
    QVector<QString> sys_notes;
    QString note;
    char *notes_ids[] = { "mtag_invite", "f_request", NULL  };
    char *nid = NULL;
    TidyNode node = NULL;
    TidyNode node2 = NULL;
    TidyBuffer tbuf;

    for(int i = 0; i < 16 ; i++) {
        nid = notes_ids[i];
        if(nid == NULL) {
            break;
        }

        node = this->searchNode(doc, NULL, nid, TidyTag_DIV);
        if(node != NULL) {
            tidyBufInit(&tbuf);

            tidyNodeGetText(doc, node, &tbuf);
            note = this->u8codec->toUnicode(QByteArray((char*)tbuf.bp));
            sys_notes.append(note);

            tidyBufFree(&tbuf);

            q_debug()<<note;
        }else{
            q_debug()<<"Warning: "<<nid<<" not found";
        }
    }

    return sys_notes;
}
QString UCHome_Main_SiteConst::find_sys_notice_by_type(TidyDoc doc, char *type_class)
{
    QString note;

    char *nid = type_class;
    TidyNode node = NULL;
    TidyNode node2 = NULL;
    TidyBuffer tbuf;

    if(nid == NULL) {
        q_debug()<<"";
        return note;
    }

    node = this->searchNode(doc, NULL, nid, TidyTag_DIV);
    if(node != NULL) {
        tidyBufInit(&tbuf);

        tidyNodeGetText(doc, node, &tbuf);
        note = this->u8codec->toUnicode(QByteArray((char*)tbuf.bp));

        tidyBufFree(&tbuf);

        q_debug()<<note;
    }else{
        q_debug()<<"Warning: "<<nid<<" not found";
    }

    
    return note;
}
////////////
static const char *notes_ids[] = { "mtag_invite", "f_request", NULL  };
TidyDoc  UCHome_Main_SiteConst::create_note_document_tree()
{
    return this->create_feed_document_tree();
}

int UCHome_Main_SiteConst::free_note_document_tree(TidyDoc doc)
{
    return this->free_feed_document_tree(doc);
}
TidyNode UCHome_Main_SiteConst::first_note(void *state_data, TidyDoc doc)
{
    ParserStateObject *state_obj = (ParserStateObject*)state_data;    
    state_obj->curr_index = 0;
    
    const char *nid = NULL;
    TidyNode node = NULL;
    TidyNode node2 = NULL;

    // nid = notes_ids[state_obj->curr_index];
    // if(nid == NULL) {
    //     return NULL;
    // }
    
    node = this->searchNode(doc, NULL, "mgs_list", TidyTag_DIV);
    if(node != NULL) {
        node2 = tidyGetChild(node);
        if(node2 != NULL && tidyNodeGetId(node2) == TidyTag_DIV) {
            return node2;
        }else{
        }
    }else{
        q_debug()<<"Warning: first note  not found";
    }

    return NULL;
}
TidyNode UCHome_Main_SiteConst::next_note(void *state_data, TidyDoc doc, TidyNode prev)
{
    ParserStateObject *state_obj = (ParserStateObject*)state_data;    
    state_obj->curr_index ++;
    
    const char *nid = NULL;
    TidyNode node = NULL;
    TidyNode node2 = NULL;

    node = tidyGetNext(prev);
    if(node != NULL) {
        return node;
    }

    return NULL;    
}
FeedRecord* UCHome_Main_SiteConst::parse_note(void *state_data, TidyDoc doc, TidyNode node)
{
    FeedRecord * rec = NULL;
    QString note;
    TidyBuffer tbuf;
    tidyBufInit(&tbuf);

    rec = new FeedRecord();
    tidyNodeGetText(doc, node, &tbuf);
    //q_debug()<<"Orig:"<<QByteArray((char*)tbuf.bp);
    md5CheckSum((char*)tbuf.bp, tbuf.size, rec->md5sum);
    tidyBufFree(&tbuf); 

    QByteArray node_string = this->save_node_with_absolute_link(doc, node);
    //q_debug()<<"Orig:"<<node_string;
    rec->content = this->u8codec->toUnicode(node_string);    
    //q_debug()<<"UUU:"<<rec->content;

    // tidyNodeGetText(doc, node, &tbuf);
    // note = this->u8codec->toUnicode(QByteArray((char*)tbuf.bp));
    // rec = new FeedRecord();
    // rec->content = this->rewrite_relative_link_ex(note);    
    // //rec->content = this->rewrite_relative_img(rec->content);
    // md5CheckSum((char*)tbuf.bp, tbuf.size, rec->md5sum);

    // q_debug()<<"Orig:"<<QByteArray((char*)tbuf.bp);
    // // q_debug()<<"rewrited:"<<rec->content ;
    // tidyBufFree(&tbuf); 
    
    return rec;
}
void * UCHome_Main_SiteConst::create_note_parser_state_data()
{
    return this->create_parser_state_data();
}
int UCHome_Main_SiteConst::free_note_parser_state_data(void *data)
{
    return this->free_parser_state_data(data);
}
QByteArray UCHome_Main_SiteConst::save_node_with_absolute_link(TidyDoc doc, TidyNode node)
{
    QByteArray ba, be;
    int nid;
    TidyNode n2;
    TidyAttr attr;
    TidyBuffer tbuf = {0};
    QString linkBase = this->home_page;
    QString linkHome = linkBase.left(linkBase.length() - QUrl(linkBase).path().length());
    QString linkPath = linkBase.left(linkBase.lastIndexOf("/")+1);
    QByteArray tval;

    //q_debug()<<linkBase<<linkHome<<linkPath;
    nid = tidyNodeGetId(node);
    ba = "<";
    ba += QByteArray(tidyNodeGetName(node));
    
    for(attr = tidyAttrFirst(node); attr ; attr = tidyAttrNext(attr)) {
        ba += QByteArray(" ");
        ba += QByteArray(tidyAttrName(attr));
        ba += QByteArray("=\"");
        if( (nid == TidyTag_A && tidyAttrGetId(attr) == TidyAttr_HREF)
            || (nid == TidyTag_IMG && tidyAttrGetId(attr) == TidyAttr_SRC) ) {
            tval = QByteArray(tidyAttrValue(attr));
            if(QString(tval).startsWith("http://", Qt::CaseInsensitive)) {
            }else if(tval.startsWith("/")) {
                ba += linkHome.toAscii();
            }else if(tval.startsWith("#")) {
                ba += linkPath.toAscii();
            }else if(QString(tval).startsWith("javascript:", Qt::CaseInsensitive)) {
                
            }else{
                ba += linkBase.toAscii();
            }
            //q_debug()<<"fill path";
        }
        ba += QByteArray(tidyAttrValue(attr));
        ba += QByteArray("\" ");
    }
    ba += QByteArray(">");

    for(n2 = tidyGetChild(node); n2 ; n2 = tidyGetNext(n2)) {
        ctmbstr name;
        switch ( tidyNodeGetType(n2) )
        {
        case TidyNode_Root:       name = "Root";                    break;
        case TidyNode_DocType:    name = "DOCTYPE";                 break;
        case TidyNode_Comment:    name = "Comment";                 break;
        case TidyNode_ProcIns:    name = "Processing Instruction";  break;
        case TidyNode_CDATA:      name = "CDATA";                   break;
        case TidyNode_Section:    name = "XML Section";             break;
        case TidyNode_Asp:        name = "ASP";                     break;
        case TidyNode_Jste:       name = "JSTE";                    break;
        case TidyNode_Php:        name = "PHP";                     break;
        case TidyNode_XmlDecl:    name = "XML Declaration";         break;
        case TidyNode_Text:       
            name = "Text";                    
            tidyBufInit(&tbuf);
            tidyNodeGetText(doc, n2, &tbuf);
            ba += QByteArray((char*)tbuf.bp);
            tidyBufFree(&tbuf);
            break;
        case TidyNode_Start:
        case TidyNode_End:
        case TidyNode_StartEnd:
        default:
            //name = tidyNodeGetName( child );
            ba += save_node_with_absolute_link(doc, n2);
            break;
        }
        //assert( name != NULL );
    }

    ba += QByteArray("</");
    ba += QByteArray(tidyNodeGetName(node));
    ba += QByteArray(">");
    return ba;
}
QByteArray UCHome_Main_SiteConst::save_node_with_absolute_link_and_filter(TidyDoc doc, TidyNode node, int tagid, char *class_or_id)
{
    QByteArray ba, be;
    int nid;
    TidyNode n2;
    TidyAttr attr;
    TidyBuffer tbuf = {0};
    QString linkBase = this->home_page;
    QString linkHome = linkBase.left(linkBase.length() - QUrl(linkBase).path().length());
    QString linkPath = linkBase.left(linkBase.lastIndexOf("/")+1);
    QByteArray tval;

    //q_debug()<<linkBase<<linkHome<<linkPath;
    nid = tidyNodeGetId(node);
    ba = "<";
    ba += QByteArray(tidyNodeGetName(node));
    
    for(attr = tidyAttrFirst(node); attr ; attr = tidyAttrNext(attr)) {
        ba += QByteArray(" ");
        ba += QByteArray(tidyAttrName(attr));
        ba += QByteArray("=\"");
        if( (nid == TidyTag_A && tidyAttrGetId(attr) == TidyAttr_HREF)
            || (nid == TidyTag_IMG && tidyAttrGetId(attr) == TidyAttr_SRC) ) {
            tval = QByteArray(tidyAttrValue(attr));
            if(QString(tval).startsWith("http://", Qt::CaseInsensitive)) {
            }else if(tval.startsWith("/")) {
                ba += linkHome.toAscii();
            }else if(tval.startsWith("#")) {
                ba += linkPath.toAscii();
            }else if(QString(tval).startsWith("javascript:", Qt::CaseInsensitive)) {
                
            }else{
                ba += linkBase.toAscii();
            }
            //q_debug()<<"fill path";
        }
        ba += QByteArray(tidyAttrValue(attr));
        ba += QByteArray("\" ");
    }
    ba += QByteArray(">");

    for(n2 = tidyGetChild(node); n2 ; n2 = tidyGetNext(n2)) {
        ctmbstr name = NULL;
        ctmbstr class_value = NULL;
        //在这过滤
        //<a href="cp.php?ac=feed&op=ignore&icon=1010415&uid=817893&feedid=2533517" id="a_feedicon_2533517" onclick="ajaxmenu(event, this.id, 99999)" class="float_cancel" title="屏蔽">屏蔽</a>
        //q_debug()<<TidyTag_A<<class_value<<tagid<<class_or_id<<tidyNodeGetName(n2);
        if(tidyNodeGetId(n2) == tagid) {
            //q_debug()<<class_value<<tagid<<class_or_id;
            attr = tidyAttrGetById(n2, TidyAttr_CLASS);
            if(attr != NULL) {
                class_value = tidyAttrValue(attr);
                //q_debug()<<class_value<<tagid<<class_or_id;
                if(class_value != NULL && qstricmp(class_value, class_or_id) == 0) {
                    //q_debug()<<"filter it, drop it";
                    continue;
                }
            }
        }
        switch ( tidyNodeGetType(n2) )
        {
        case TidyNode_Root:       name = "Root";                    break;
        case TidyNode_DocType:    name = "DOCTYPE";                 break;
        case TidyNode_Comment:    name = "Comment";                 break;
        case TidyNode_ProcIns:    name = "Processing Instruction";  break;
        case TidyNode_CDATA:      name = "CDATA";                   break;
        case TidyNode_Section:    name = "XML Section";             break;
        case TidyNode_Asp:        name = "ASP";                     break;
        case TidyNode_Jste:       name = "JSTE";                    break;
        case TidyNode_Php:        name = "PHP";                     break;
        case TidyNode_XmlDecl:    name = "XML Declaration";         break;
        case TidyNode_Text:       
            name = "Text";                    
            tidyBufInit(&tbuf);
            tidyNodeGetText(doc, n2, &tbuf);
            ba += QByteArray((char*)tbuf.bp);
            tidyBufFree(&tbuf);
            break;
        case TidyNode_Start:
        case TidyNode_End:
        case TidyNode_StartEnd:
        default:
            //name = tidyNodeGetName( child );
            ba += save_node_with_absolute_link_and_filter(doc, n2, tagid, class_or_id);
            break;
        }
        //assert( name != NULL );
    }

    ba += QByteArray("</");
    ba += QByteArray(tidyNodeGetName(node));
    ba += QByteArray(">");
    return ba;
}

QString UCHome_Main_SiteConst::rewrite_relative_link(QString html)
{
    int pos = 0;
    QString tmp = html;
    QString str1;
    QString href;
    QRegExp arex( "(href[ ]*=[ ]*\")(.*)\"");
    arex.setMinimal(true);
    QString linkBase = this->home_page;
    QString linkHome = linkBase.left(linkBase.length() - QUrl(linkBase).path().length());
    QString linkPath = linkBase.left(linkBase.lastIndexOf("/")+1);
    //q_debug()<<linkHome<<linkPath;

    q_debug()<<"before rewrite:"<<tmp;
    tmp = tmp.replace("\n", "");
    while(true) {
        //pos = tmp.indexOf(href, pos, Qt::CaseInsensitive);
        pos = arex.indexIn(tmp, pos);
        if(pos == -1) break;
        str1 = arex.cap(1);
        href = arex.cap(2);
        if(href.startsWith("http")) {
            pos += arex.matchedLength();
        }else if(href.startsWith("javascript:", Qt::CaseInsensitive)) {
            pos += arex.matchedLength();
        }else if(href.startsWith(QChar('/'))) {
            tmp = tmp.insert(pos+str1.length(), linkHome);
            pos += arex.matchedLength() + linkHome.length();
        }else if(href.startsWith(QChar('#'))) {
            tmp = tmp.insert(pos+str1.length(), linkBase);
            pos += arex.matchedLength() + linkBase.length();
        }else{
            tmp = tmp.insert(pos+str1.length(), linkPath);
            pos += arex.matchedLength() + linkPath.length();
        }
    }
    q_debug()<<"end rewrite:"<<tmp;
    //q_debug()<<tmp<<"\n";
    return tmp;    
}
QString UCHome_Main_SiteConst::rewrite_relative_link_ex(QString html)
{

    int pos = 0, opos = 0;
    QString tmp = html;
    QString str1, str2;
    QString href;
    QRegExp arex( "(href[ ]*=[ ]*\")(.*)\"");
    arex.setMinimal(true);
    QString linkBase = this->home_page;
    QString linkHome = linkBase.left(linkBase.length() - QUrl(linkBase).path().length());
    QString linkPath = linkBase.left(linkBase.lastIndexOf("/")+1);
    //q_debug()<<linkHome<<linkPath;

    tmp = tmp.replace("\n", "");
    q_debug()<<"before rewrite:"<<tmp;
    while(true) {
        //pos = tmp.indexOf(href, pos, Qt::CaseInsensitive);
        pos = arex.indexIn(tmp, pos);
        if(pos == -1) break;
        str2 += tmp.mid(opos, pos - opos);
        str1 = arex.cap(1);
        href = arex.cap(2);
        str2 += str1;
        opos = pos + str1.length();
        if(href.startsWith("http")) {
            pos += arex.matchedLength();
        }else if(href.startsWith("javascript:", Qt::CaseInsensitive)) {
            pos += arex.matchedLength();
        }else if(href.startsWith(QChar('/'))) {
            //tmp = tmp.insert(pos+str1.length(), linkHome);
            str2 += linkHome;
            //pos += arex.matchedLength() + linkHome.length();
        }else if(href.startsWith(QChar('#'))) {
            str2 += linkBase;
            pos += arex.matchedLength();
            //tmp = tmp.insert(pos+str1.length(), linkBase);
            //pos += arex.matchedLength() + linkBase.length();
        }else{
            str2 += linkPath;
            pos += arex.matchedLength();
            //tmp = tmp.insert(pos+str1.length(), linkPath);
            //pos += arex.matchedLength() + linkPath.length();
        }
        q_debug()<<"PROCESS: "<<str2;
    }
    str2 += tmp.mid(opos, tmp.length() - opos);
    q_debug()<<"end rewrite:"<<str2;
    //q_debug()<<tmp<<"\n";

    return str2;
    return tmp;    

}
QString UCHome_Main_SiteConst::rewrite_relative_img(QString html)
{

    int pos = 0;
    QString tmp = html;
    QString str1;
    QString href;
    QRegExp arex( "(src[ ]*=[ ]*\")(.*)\"");
    arex.setMinimal(true);
    QString linkBase = this->home_page;
    QString linkHome = linkBase.left(linkBase.length() - QUrl(linkBase).path().length());
    QString linkPath = linkBase.left(linkBase.lastIndexOf("/")+1);
    //q_debug()<<linkHome<<linkPath;

    tmp = tmp.replace("\n", "");
    while(true) {
        //pos = tmp.indexOf(href, pos, Qt::CaseInsensitive);
        pos = arex.indexIn(tmp, pos);
        if(pos == -1) break;
        str1 = arex.cap(1);
        href = arex.cap(2);
        if(href.startsWith("http")) {
            pos += arex.matchedLength();
        }else if(href.startsWith("javascript:", Qt::CaseInsensitive)) {
            pos += arex.matchedLength();
        }else if(href.startsWith(QChar('/'))) {
            tmp = tmp.insert(pos+str1.length(), linkHome);
            pos += arex.matchedLength() + linkHome.length();
        }else if(href.startsWith(QChar('#'))) {
            tmp = tmp.insert(pos+str1.length(), linkBase);
            pos += arex.matchedLength() + linkBase.length();
        }else{
            tmp = tmp.insert(pos+str1.length(), linkPath);
            pos += arex.matchedLength() + linkPath.length();
        }
    }
    //q_debug()<<tmp<<"\n";
    return tmp;    
}


int UCHome_Main_SiteConst::fetchFriendData()
{
    q_debug()<<"run here";

    QString url;
    int friend_list_page_count;
    int frined_list_count;
    QByteArray html;
    int rc ;

    url = this->friend_url;
    q_debug()<<url;
    rc = this->get_url(url, html);
    if(rc != FETCH_FEED_OK) {
        q_debug()<<"get url error";
        return rc;
    }
    
    this->friend_page_html = html;
    this->friend_page_utf8_html = this->u8codec->fromUnicode(this->codec->toUnicode(html));

    return FETCH_FEED_OK;
    return 0;
}

QString UCHome_Main_SiteConst::parse_userid(TidyDoc doc)
{
    q_debug()<<"not impled, no use now";
    TidyNode node;
    TidyAttr  attr;
    ctmbstr   uidstr;
    QString uid;

    node = this->searchNode(doc, NULL, "nav_account", TidyTag_DIV);
    if(node != NULL) {
        node = tidyGetChild(node);
        if(node != NULL) {
            node = tidyGetChild(node);
            if(node != NULL) {
                Q_ASSERT(tidyNodeGetId(node) == TidyTag_IMG);
                attr = tidyAttrGetById(node, TidyAttr_SRC);
                if(attr != NULL) {
                    uidstr = tidyAttrValue(attr);
                    QString tmp = QString(uidstr);
                    int p = tmp.indexOf("uid=");
                    int p2 = tmp.indexOf("&", p);
                    uid = tmp.mid(p+4, p2 - p - 4);
                }
            }
        }
    }
    if(uid.isEmpty()) {
        q_debug()<<"Cannot found uid no.";
    }else{
        q_debug()<<"UserID: "<<uid;
    }
    
    return uid;
}

TidyDoc  UCHome_Main_SiteConst::create_friend_document_tree()
{
    TidyDoc doc = NULL;

    doc = this->create_document_tree(this->friend_page_utf8_html);
    
    return doc;
}
int UCHome_Main_SiteConst::free_friend_document_tree(TidyDoc doc)
{
    this->free_document_tree(doc);
    return 0;
}
TidyNode UCHome_Main_SiteConst::first_friend(void *state_data, TidyDoc doc) 
{
    TidyNode node = NULL;
    TidyNode node2 = NULL;
    TidyNode node3 = NULL;

    node = this->searchNode(doc, NULL, "friend_ul", TidyTag_DIV);
    if(node != NULL) {
        node2 = tidyGetChild(node);
        if(tidyNodeGetId(node2) == TidyTag_UL) {
            node3 = tidyGetChild(node2);
            if(node3 != NULL && tidyNodeGetId(node3) == TidyTag_LI) {
                return node3;
            }else{
                q_debug()<<"no friends?????";
            }
        }else{
            q_debug()<<"no friends?????";
        }
    }else{
        q_debug()<<"no friends?????";
    }
    return NULL;
}
TidyNode UCHome_Main_SiteConst::next_friend(void *state_data, TidyDoc doc, TidyNode prev) 
{
    TidyNode node = NULL;
    TidyNode node2 = NULL;

    node = tidyGetNext(prev);

    if(node != NULL) {
        return node;
        // node2 = this->searchNode(doc, node, NULL, TidyTag_SELECT);
        // if(node2 == NULL) {
        //     return node;
        // }else{
        //     //最后一条 tr元素
        // }
    }

    return NULL;
}
FeedRecord* UCHome_Main_SiteConst::parse_friend(void *state_data, TidyDoc doc, TidyNode node) 
{
    FeedRecord * rec = NULL;
    QString note;
    TidyBuffer tbuf;
    TidyNode node2 = NULL;
    TidyNode node3 = NULL;
    TidyNode node4 = NULL;
    TidyNode node5 = NULL;
    TidyAttr attr = NULL;
    ctmbstr fuid = NULL;
    ctmbstr fusername = NULL;

    node2 = this->searchNode(doc, node, "thumbTitle", TidyTag_DIV);
    if(node2 != NULL) {
        rec = new FeedRecord();
        node3 = tidyGetChild(node2);
        if(node3 != NULL) {
            attr = tidyAttrGetById(node3, TidyAttr_VALUE);
            Q_ASSERT(attr != NULL);
            fuid = tidyAttrValue(attr);
            rec->fuid = QString(fuid).trimmed();
            node4 = tidyGetNext(node3);
            if(node4 != NULL) {
                tidyBufInit(&tbuf);
                tidyNodeGetText(doc, tidyGetChild(node4), &tbuf);
                rec->fusername = this->u8codec->toUnicode(QByteArray((char*)tbuf.bp).trimmed());                
                tidyBufFree(&tbuf);
            }
            md5CheckSum(fuid, strlen(fuid), rec->md5sum);//使用uid的md5值肯定不会出现冲突
        }
    }

    q_debug()<<"USER:"<<fuid<<rec->fusername;

    //tidyBufInit(&tbuf);
    // tidyNodeGetText(doc, node, &tbuf);
    // note = this->u8codec->toUnicode(QByteArray((char*)tbuf.bp));    
    // rec = new FeedRecord();
    // rec->content = this->rewrite_relative_link(note);
    // rec->content = "<table>" + rec->content + "</table>";
    // md5CheckSum((char*)tbuf.bp, tbuf.size, rec->md5sum);
    // tidyBufFree(&tbuf);    
    // //q_debug()<<"Orig:"<<note;

    // node2 = tidyGetChild(node);
    // node3 = tidyGetChild(node2);
    // attr = tidyAttrGetById(node3, TidyAttr_VALUE);
    // fuid = tidyAttrValue(attr);
    
    // node4 = tidyGetNext(node2);
    // node3 = tidyGetChild(node4); // A
    // node5 = tidyGetChild(node3); //IMG
    // attr = tidyAttrGetById(node5, TidyAttr_ALT);
    // fusername = tidyAttrValue(attr);

    // rec->fuid = fuid;
    // rec->fusername = this->u8codec->toUnicode(QByteArray(fusername));
    // q_debug()<<"USER:"<<fuid<<rec->fusername;
    // md5CheckSum(fuid, strlen(fuid), rec->md5sum);//使用uid的md5值肯定不会出现冲突
    
    return rec;
}
void * UCHome_Main_SiteConst::create_friend_parser_state_data()  
{
    return NULL;
}
int UCHome_Main_SiteConst::free_friend_parser_state_data(void *data) 
{    
    return 0;
}

QString UCHome_Main_SiteConst::avataUrl(QString uid, int type) 
{
    QString url;
    //http://www.discuz.net/uc_server/avatar.php?uid=817893&size=small&type=virtual
    
    url = QString("http://www.discuz.net/uc_server/avatar.php?uid=%1&size=%2&type=virtual").arg(uid);
    switch(type) {
    case AVATA_SMALL:
        url = url.arg("small");
        break;
    case AVATA_MIDDLE:
        url = url.arg("middle");
        break;
    case AVATA_BIG:
        url = url.arg("small");
        break;
    default:
        url = url.arg("middle");
        q_debug()<<"Warning: which type avata, small or big?";
        break;
    }    

    return url;
}
QString UCHome_Main_SiteConst::homePageUrl(QString uid)
{
    QString url;
    //http://uchome.developer.manyou.com/uchome/space.php?uid=994935
    url = QString("http://u.discuz.net/home/space.php?uid=%1").arg(uid);
    
    return url;    
}
void UCHome_Main_SiteConst::wkLoadStarted()
{
    q_debug()<<this->webframe->url();    
    emit this->showLoginMessage(tr("Load login page."));
}
void UCHome_Main_SiteConst::wkLoadFinished ( bool ok )
{
    QString us = this->webframe->url().toString();
    q_debug()<<ok;
    q_debug()<<us<<this->webpage->totalBytes();
    //<<this->webpage->mainFrame()->toHtml();
    QList<QString> cookie_names;
    QStringList rawCookieList;
    QNetworkCookieJar * cjar = this->nam->cookieJar();
    QList<QNetworkCookie> cookies = cjar->cookiesForUrl(this->home_page);
    for(int i = 0 ; i < cookies.count(); i ++) {
        q_debug()<<cookies.at(i).toRawForm();
        cookie_names.append(cookies.at(i).name());
        rawCookieList.append(cookies.at(i).toRawForm());
    }
    if(us == this->home_page) {
        QString jscmd = QString("document.loginform.username.value='%1'; document.loginform.password.value='%2';")
            .arg(this->username, this->password);
        QVariant jsval = this->webframe->evaluateJavaScript(jscmd);        
        jsval = this->webframe->evaluateJavaScript("document.loginform.username.value;");        
        q_debug()<<jsval;
        jsval = this->webframe->evaluateJavaScript("document.loginform.password.value;");        
        q_debug()<<jsval;
        jsval = this->webframe->evaluateJavaScript("document.loginform.loginsubmit.click();");
        q_debug()<<jscmd<<jsval;
    }else if(us.endsWith("&&ref")) {
        q_debug()<<"ready for enter url";
        TidyDoc tdoc = NULL;
        TidyNode node1 = NULL;
        TidyNode node2 = NULL;
        TidyNode node3 = NULL;
        TidyBuffer tbuf = {0};
        QByteArray u8html ;
        QString u16html ;
        QByteArray u8question = NULL;
        int answer = 88888;

        u16html = this->webframe->toHtml();
        u8html = u16html.toUtf8();
        u16html.clear();

        tdoc = this->create_document_tree(u8html);//u16
        Q_ASSERT(tdoc != NULL);
        node1 = this->searchNode(tdoc, NULL, "seccode", TidyTag_INPUT);
        Q_ASSERT(node1 != NULL);
        node2 = tidyGetPrev(node1);
        Q_ASSERT(node2 != NULL);
        Q_ASSERT(tidyNodeGetId(node2) == TidyTag_P);
        tidyBufInit(&tbuf);
        tidyNodeGetText(tdoc, tidyGetChild(node2), &tbuf);
        u8question = QByteArray((char*)tbuf.bp).trimmed();
        q_debug()<<"q: "<<u8question<<", answer: "<<answer;
        answer = this->uch_question_caculor(u8question);        
        q_debug()<<"q: "<<u8question<<", answer: "<<answer;
        
        QString jscmd = QString("document.loginform.seccode.value='%1'; ")
            .arg(QString("%1").arg(answer));
        QVariant jsval = this->webframe->evaluateJavaScript(jscmd);        
        q_debug()<<jscmd<<this->webframe->evaluateJavaScript("document.loginform.seccode.value;");
        jscmd = QString("document.loginform.action+='&post2=true'; document.loginform.action;");
        jsval = this->webframe->evaluateJavaScript(jscmd);        
        q_debug()<<jsval;
        //q_debug()<<this->webframe->toHtml();
        jsval = this->webframe->evaluateJavaScript("document.loginform.loginsubmit.click();");
        
        /////clean up document
        tidyBufFree(&tbuf);
        this->free_document_tree(tdoc);
    }else if(us.endsWith("post2=true")) {        
        q_debug()<<"here";
        if(cookie_names.contains("uchome_auth")) {
            q_debug()<<"here";
            emit this->showLoginMessage(QString(tr("Login finished successfully.")));
            this->combine_cookies2(rawCookieList);
            emit this->loginDone(LOGIN_OK);
            this->webpage->history()->clear();
            //还有用，不能清理掉啊
            //delete this->webpage; this->webpage = NULL;
            //this->webframe = NULL;
            this->webframe->setUrl(QUrl("about: blank"));
            this->webpage->triggerAction(QWebPage::Stop, true);
            
            // set login off flag
            this->isLoging.setBit(0, false);
        }else{
            if(this->loginRetryTimes > 0) {
                this->loginRetryTimes -= 1;
                this->webframe->setUrl(this->home_page);
                q_debug()<<"Login retry :"<<this->loginRetryTimes;
                emit this->showLoginMessage(QString(tr("Loading: retry %1.")).arg(this->loginRetryTimes));
            }else{
                emit this->loginDone(LOGIN_ERROR);
                this->isLoging.setBit(0, false);            
            }
        }
    }else if(us.endsWith("/cp.php?ac=pm")) {
        //q_debug()<<this->webframe->toHtml();//注意有时候用户没有激活邮箱不能发消息处理。     
        //您需要添加 2 个好友之后，才能进行本操作             
        QVector<QString> me;
        QString uid;
        QString msg;
        if(this->msgQueue.count() > 0) {
            me = this->msgQueue.dequeue();
            uid = me.at(0);
            msg = me.at(1);
            QString jscmd ;
            QVariant jsval;

            jscmd = QString("document.pmform_0.username.value='%1'; document.pmform_0.username.value;").arg(uid);
            jsval = this->webframe->evaluateJavaScript(jscmd);
            q_debug()<<jscmd<<jsval;

            jscmd = QString("document.pmform_0.message.value='%1'; document.pmform_0.message.value;").arg(msg);
            jsval = this->webframe->evaluateJavaScript(jscmd);
            q_debug()<<jscmd<<jsval;

            jscmd = QString("document.pmform_0.username.value ; document.pmform_0.message.value;");
            jsval = this->webframe->evaluateJavaScript(jscmd);
            q_debug()<<jsval;
            jscmd = QString("document.pmform_0.pmsubmit_btn.click();");
            jsval = this->webframe->evaluateJavaScript(jscmd);
        }
    }else if(us.endsWith("/cp.php?ac=pm&op=send&touid=0&pmid=0")) {
        this->webframe->setUrl(QUrl("about: blank"));
        this->webpage->triggerAction(QWebPage::Stop, true);
        if(this->msgQueue.count() > 0) {
            this->webframe->setUrl(this->home_page + "cp.php?ac=pm");
        }
    } else {
        //next step 
        q_debug()<<"next do what?"<<this->webframe->url();
        if(this->isLoging.at(0) == true && this->cookies.length() == 0) {
            //must login error
            emit this->loginDone(LOGIN_ERROR);
            this->isLoging.setBit(0, false);
        }
    }
}
void UCHome_Main_SiteConst::wkLoadProgress ( int progress )
{
    //q_debug()<<progress;
    qDebug()<<progress;
    // QWebFrame *frm = this->webpage->mainFrame();
    // q_debug()<<frm->renderTreeDump();
    if(this->loginRetryTimes == 3) {
        emit this->showLoginMessage(QString(tr("Loading: %1.")).arg(progress));
    }else{
        emit this->showLoginMessage(QString(tr("Retry %1 Loading: %2."))
                                    .arg(QString(int(this->loginRetryTimes)), QString(progress)));
    }
}

bool UCHome_Main_SiteConst::sendMsg(QString uid, QString msg)
{
    QVector<QString> me;
    me.append(uid);
    me.append(msg);

    this->msgQueue.enqueue(me);
    this->webframe->setUrl(QUrl(this->home_page + "cp.php?ac=pm"));

    return true;
}

//////////////
////////////// interface for module
extern "C" MY_EXPORT SiteConst * get_site_const(int step)
{
    SiteConst *sc = NULL;

    sc = new UCHome_Main_SiteConst();

    return sc;
}

extern "C" MY_EXPORT int release_site_const(SiteConst **sc)
{
    assert(*sc != NULL);
    delete *sc;
    sc = NULL;

    return 0;
}


