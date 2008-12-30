#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>

#include <QtCore>
#include <QtNetwork>

#include "md5.h"
#include <tidy.h>
#include "tidy_wrapper.h"
#include "feed_record.h"
#include "sns_feed.h"
#include "utils.h"

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

/////////define
class UCHome_Main_SiteConst : public SiteConst
{
public:
    UCHome_Main_SiteConst();
    ~UCHome_Main_SiteConst();

    //interface
    virtual int get_version();
    virtual int login() ;
    virtual int logout() ;

    virtual int parse_userid();

    virtual int fetchFeedData() ;
    virtual int fetchFriendData();

    virtual TidyNode first_feed( void *state_data, TidyDoc doc);
    virtual TidyNode next_feed( void *state_data, TidyDoc doc, TidyNode prev);
    virtual FeedRecord* parse_feed( void *state_data, TidyDoc doc, TidyNode node);    

    virtual void * create_parser_state_data() ;
    virtual int free_parser_state_data(void *data);
    virtual QString format_human_feed(FeedRecord *rec);

    //////
    virtual TidyDoc  create_note_document_tree();
    virtual int free_note_document_tree(TidyDoc doc);
    virtual TidyNode first_note(void *state_data, TidyDoc doc);
    virtual TidyNode next_note(void *state_data, TidyDoc doc, TidyNode prev);
    virtual FeedRecord* parse_note(void *state_data, TidyDoc doc, TidyNode node);
    virtual void * create_note_parser_state_data();
    virtual int free_note_parser_state_data(void *data);

    ////////    
    virtual TidyDoc  create_friend_document_tree();
    virtual int free_friend_document_tree(TidyDoc doc);
    virtual TidyNode first_friend(void *state_data, TidyDoc doc) ;
    virtual TidyNode next_friend(void *state_data, TidyDoc doc, TidyNode prev) ;
    virtual FeedRecord* parse_friend(void *state_data, TidyDoc doc, TidyNode node) ;
    virtual void * create_friend_parser_state_data()  ;
    virtual int free_friend_parser_state_data(void *data) ;

    ////////private
private:
    void clean_my_name_lable();
    QString get_time_string(TidyDoc doc, TidyNode tnode);
    QString find_nick_name(TidyDoc doc);
    QString find_sign_text(TidyDoc doc);
    QString find_photo_url(TidyDoc doc);
    QVector<QString> find_sys_notice(TidyDoc doc);
    QString find_sys_notice_by_type(TidyDoc doc, char *type_class);
    QString rewrite_relative_link(QString src);

};

/////////impl
UCHome_Main_SiteConst::UCHome_Main_SiteConst()
{
    site_name = "manyou";
    site_icon = "faviocn.ico";
    home_page = "http://uchome.developer.manyou.com/uchome/";
    encoding = "gbk";
    host = "uchome.developer.manyou.com";
    port = 80;
    login_url = "/uchome/do.php?ac=login&&ref";
    username_field_name="username";
    password_field_name="password";
    data = QString::null;
    cookies = QString::null;
    username = QString::null;
    password = QString::null;
    feed_url = "/uchome/space.php?do=home&view=we";
    other_fields.append(QPair<QString,QString>("cookietime", "315360000"));
    step = 0;
    next = 0;  // 0, 1
    //char **letter_urls;
    letter_urls.append("");
    //int  (*letter_parsers[10])();    

    friend_url = "http://uchome.developer.manyou.com/uchome/cp.php?ac=friend&op=group";

    codec = QTextCodec::codecForName(encoding.toAscii().data());
}
UCHome_Main_SiteConst::~UCHome_Main_SiteConst()
{
}

int UCHome_Main_SiteConst::get_version()
{
    return 123;
}

int UCHome_Main_SiteConst::login()
{
    QTcpSocket *sock = NULL;
    
    sock = new QTcpSocket();
    sock->connectToHost(this->host, this->port);
    if(!sock->waitForConnected()) {
        q_debug()<<"error:";
        return LOGIN_ERROR;
    }

    QString data ;
    data = QString("username=%1&password=%2&cookietime=315360000&refer=space.php%3Fdo%3Dhome&loginsubmit=%B5%C7%C2%BC&formhash=3040598c").arg(this->username, this->password) ;
    
    QString request;
    request = 
        QString("POST /uchome/do.php?ac=login&&ref HTTP/1.1\r\n"
                "Host: uchome.developer.manyou.com\r\n"
                "User-Agent: Mozilla/5.0 (X11; U; Linux i686; zh-CN; rv:1.9.0.4) Gecko/2008112913 Gentoo Minefield/3.0.4\r\n"
                "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
                "Accept-Language: zh-cn,zh;q=0.5\r\n"
                "Accept-Charset: gb2312,utf-8;q=0.7,*;q=0.7\r\n"
                "Keep-Alive: 300\r\n"
                "Connection: keep-alive\r\n"
                "Referer: http://uchome.developer.manyou.com/uchome/index.php\r\n"
                "Content-Type: application/x-www-form-urlencoded\r\n"
                "Content-Length: %1\r\n\r\n")
        .arg(data.length());

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
    q_debug()<<html.left(6)<<html.right(20);

    this->clean_my_name_lable();

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
    this->get_node_text(doc, node, &tbuf);
    feed_bytes = QByteArray((char*)tbuf.bp);
    feed_text = this->u8codec->toUnicode(feed_bytes);

    tidyBufFree(&tbuf);

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
    //<my:name uid="1673222" />
    //<my:name uid="1673222"></my:name>

    //QString unis = this->codec->toUnicode(this->feed_page_html);
    //QByteArray u8s = this->u8codec->fromUnicode(unis);
    //unis = u8s;
    QByteArray tarr = this->feed_page_html;

    QRegExp exp1 ("<my:name uid=\"[0-9]{1,12}\" />");
    QRegExp exp2 ( "<my:name uid=\"[0-9]{1,12}\"></my:name>");
    
    tarr = tarr.trimmed();
    if(tarr.startsWith("f5b")) {
        tarr = tarr.right(tarr.length()-3);
    }
    if(tarr.endsWith("0")) {
        tarr = tarr.left(tarr.length()-1);
    }
    //unis = unis.replace(exp1, QString(""));
    //unis = unis.replace(exp2, QString(""));
    
    //u8s = this->u8codec->fromUnicode(unis);
    QString unis = this->codec->toUnicode(tarr);

    this->feed_page_utf8_html = this->u8codec->fromUnicode(unis);

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
    TidyBuffer tbuf;
    
    node = this->searchNode(doc, NULL, "state", TidyTag_DIV);
    if(node != NULL) {
        node2 = tidyGetChild(node);
        Q_ASSERT(tidyNodeGetId(node2) == TidyTag_A);
        tidyBufInit(&tbuf);
        if(tidyNodeGetText(doc, tidyGetChild(node2), &tbuf)) {
            sign_text = this->u8codec->toUnicode(QByteArray((char*)tbuf.bp));
            sign_text = sign_text.trimmed();
            q_debug()<<"Sign text:"<<sign_text;
        }
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
    ctmbstr url_str = NULL;
    TidyAttr  attr = NULL;
    
    //quick_update s_clear
    node = this->searchNode(doc, NULL, "quick_update s_clear", TidyTag_DIV);
    if(node != NULL) {
        node2 = tidyGetChild(node);
        Q_ASSERT(tidyNodeGetId(node2) == TidyTag_IMG);
        attr = tidyAttrGetById(node2, TidyAttr_SRC);
        if(attr != NULL) {
            url_str = tidyAttrValue(attr);
            photo_url = QString(url_str);
            photo_url = photo_url.replace("small", "big");
            q_debug()<<"Photo url: "<<photo_url;
        }else{
        }
    }else{
        q_debug()<<"Warning: no photo url found";
    }
    
    return photo_url;
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
    TidyNode node;

    nid = notes_ids[state_obj->curr_index];
    if(nid == NULL) {
        return NULL;
    }
    
    node = this->searchNode(doc, NULL, nid, TidyTag_DIV);
    if(node != NULL) {
        return node;
    }else{
        q_debug()<<"Warning: "<<nid<<" not found";
    }

    return NULL;
}
TidyNode UCHome_Main_SiteConst::next_note(void *state_data, TidyDoc doc, TidyNode prev)
{
    ParserStateObject *state_obj = (ParserStateObject*)state_data;    
    state_obj->curr_index ++;
    
    const char *nid = NULL;
    TidyNode node;

    nid = notes_ids[state_obj->curr_index];
    if(nid == NULL) {
        return NULL;
    }
    
    node = this->searchNode(doc, NULL, nid, TidyTag_DIV);
    if(node != NULL) {
        return node;
    }else{
        q_debug()<<"Warning: "<<nid<<" not found";
    }

    return NULL;    
}
FeedRecord* UCHome_Main_SiteConst::parse_note(void *state_data, TidyDoc doc, TidyNode node)
{
    FeedRecord * rec = NULL;
    QString note;
    TidyBuffer tbuf;
    
    tidyBufInit(&tbuf);

    tidyNodeGetText(doc, node, &tbuf);
    note = this->u8codec->toUnicode(QByteArray((char*)tbuf.bp));    
    rec = new FeedRecord();
    rec->content = this->rewrite_relative_link(note);
    md5CheckSum((char*)tbuf.bp, tbuf.size, rec->md5sum);
    tidyBufFree(&tbuf);    
    //q_debug()<<"Orig:"<<note;
    
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

int UCHome_Main_SiteConst::parse_userid()
{
    q_debug()<<"not impled, no use now";
    return 0;
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

    node = this->searchNode(doc, NULL, "formtable linetable", TidyTag_TABLE);
    if(node != NULL) {
        node2 = this->searchNode(doc, node, NULL, TidyTag_TR);
        if(node2 != NULL) {
            return node2;
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
        node2 = this->searchNode(doc, node, NULL, TidyTag_SELECT);
        if(node2 == NULL) {
            return node;
        }else{
            //最后一条 tr元素
        }
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

    tidyBufInit(&tbuf);

    tidyNodeGetText(doc, node, &tbuf);
    note = this->u8codec->toUnicode(QByteArray((char*)tbuf.bp));    
    rec = new FeedRecord();
    rec->content = this->rewrite_relative_link(note);
    rec->content = "<table>" + rec->content + "</table>";
    md5CheckSum((char*)tbuf.bp, tbuf.size, rec->md5sum);
    tidyBufFree(&tbuf);    
    //q_debug()<<"Orig:"<<note;

    node2 = tidyGetChild(node);
    node3 = tidyGetChild(node2);
    attr = tidyAttrGetById(node3, TidyAttr_VALUE);
    fuid = tidyAttrValue(attr);
    
    node4 = tidyGetNext(node2);
    node3 = tidyGetChild(node4); // A
    node5 = tidyGetChild(node3); //IMG
    attr = tidyAttrGetById(node5, TidyAttr_ALT);
    fusername = tidyAttrValue(attr);

    rec->fuid = fuid;
    rec->fusername = this->u8codec->toUnicode(QByteArray(fusername));
    q_debug()<<"USER:"<<fuid<<rec->fusername;
    md5CheckSum(fuid, strlen(fuid), rec->md5sum);//使用uid的md5值肯定不会出现冲突
    
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


