#ifndef FEED_RECORD_H
#define FEED_RECORD_H

#include <QtCore>

#include <tidy.h>
#include <buffio.h>

class Account;

/////////
class FeedRecord
{
public:
    FeedRecord() { memset(this->md5sum, 0, 16);}
    ~FeedRecord() {}
    bool equalsTo(FeedRecord *rec) { return memcmp(this->md5sum, rec->md5sum, 16) == 0; }
    QString md5StringSum()
    {
        const char *hexchars = "0123456789abcdef";
        unsigned char ch;
        unsigned char chh4, chl4;
        QString md5str;
        char c;
        for(int i = 0 ; i < 16; i ++) {
            ch = this->md5sum[i];
            chh4 = ch >> 4 & 0x0F;
            chl4 = ch & 0x0F;
            Q_ASSERT(chh4 >= 0 && chh4 < 16);
            Q_ASSERT(chl4 >= 0 && chl4 < 16);
            c = hexchars[chh4];
            md5str += QChar(c);
            c = hexchars[chl4];
            md5str += QChar(c);
        }
    
        Q_ASSERT(md5str.length() == 32);
        return md5str;
    }

    Account *account;

    QString type;
    QString trends; // image, msg type image
    QString content;
    QString header; // html header
    QDateTime date;
    int day;
    unsigned char md5sum[16];

    //friend info
    QString fuid;
    QString fusername; 
    QString fnickname; //nick name
};


//////c++
class SiteConst : public QObject
{
    Q_OBJECT;
public:
    SiteConst();
    ~SiteConst();

    //interface
    virtual int get_version() = 0;
    virtual QString parse_userid(TidyDoc doc) = 0;
    
    TidyDoc  create_document_tree(QByteArray html);
    int free_document_tree(TidyDoc doc);
    
    ////
    virtual TidyDoc  create_feed_document_tree();
    virtual int free_feed_document_tree(TidyDoc doc);
    virtual TidyNode first_feed(void *state_data, TidyDoc doc) = 0;
    virtual TidyNode next_feed(void *state_data, TidyDoc doc, TidyNode prev) = 0 ;
    virtual FeedRecord* parse_feed( void *state_data, TidyDoc doc, TidyNode node) = 0;
    virtual void * create_parser_state_data() = 0 ;
    virtual int free_parser_state_data(void *data) = 0;
    virtual QString format_human_feed(FeedRecord *rec) = 0;

    ////////    
    virtual TidyDoc  create_note_document_tree() = 0;
    virtual int free_note_document_tree(TidyDoc doc) = 0;
    virtual TidyNode first_note(void *state_data, TidyDoc doc) = 0;
    virtual TidyNode next_note(void *state_data, TidyDoc doc, TidyNode prev) = 0;
    virtual FeedRecord* parse_note(void *state_data, TidyDoc doc, TidyNode node) = 0;
    virtual void * create_note_parser_state_data() = 0 ;
    virtual int free_note_parser_state_data(void *data) = 0;
    virtual QString format_human_note(FeedRecord *rec);

    ////////    
    virtual TidyDoc  create_friend_document_tree() = 0;
    virtual int free_friend_document_tree(TidyDoc doc) = 0;
    virtual TidyNode first_friend(void *state_data, TidyDoc doc) = 0;
    virtual TidyNode next_friend(void *state_data, TidyDoc doc, TidyNode prev) = 0;
    virtual FeedRecord* parse_friend(void *state_data, TidyDoc doc, TidyNode node) = 0;
    virtual void * create_friend_parser_state_data() = 0 ;
    virtual int free_friend_parser_state_data(void *data) = 0;
    virtual QString format_human_friend(FeedRecord *rec);

    virtual QString detectHtmlEncoding(TidyDoc tdoc);
    virtual QString detectHtmlEncoding(const QString & htmlStr);

    TidyNode searchNode(TidyDoc tdoc, TidyNode parentNode, const char * class_name, int label);
    TidyNode searchParentNode(TidyDoc tdoc, TidyNode childNode, const char * class_name, int label);
    TidyNode searchNext(TidyDoc tdoc, TidyNode node);
    void get_node_text(TidyDoc tdoc, TidyNode node, TidyBuffer *buf);
    void get_node_text3(TidyDoc tdoc, TidyNode node, TidyBuffer *buf,
                                   const char *date_class, TidyBuffer *dbuf);
    
    virtual QString avataUrl(QString uid, int type) = 0;
    virtual QString homePageUrl(QString uid) = 0;
    virtual bool sendMsg(QString uid, QString msg) = 0;

    QString errorString() { return this->error_message;}
    QString md5(const char *s);

    ////////
    enum {SITE_MIN = 0x0, SITE_HAINEI = 0x01, SITE_XIAONEI = 0x02, SITE_KAIXIN = 0x04, 
          SITE_UCHOME_MAIN = 0x08, SITE_UCHOME_MANYOU = 0x16, SITE_MAX};
    enum {SNS_OK, SNS_ERROR, LOGIN_OK, LOGIN_ERROR, FETCH_FEED_OK, FETCH_FEED_ERROR};
    //通知 的所有类型代码    
    enum {LET_MIN, LET_NEWBOX, LET_INBOX, LET_OUTBOX, LET_PUBMSG, LET_APPMSG, LET_COMMENT, LET_COMMENT_REPLY,
          LET_BBS, LET_BBS_REPLY, LET_NOTICE, LET_MAX};

    enum {AVATA_MIN, AVATA_SMALL, AVATA_MIDDLE, AVATA_BIG, AVATA_MAX};
    
public slots:    
    virtual int login(Account *acc) = 0;
    virtual int logout() = 0;

    virtual int fetchFeedData() = 0;
    virtual int fetchFriendData() = 0;

signals:    
    void loginDone(int status);
    void showLoginMessage(QString msg);

protected:
    QString combine_cookies(QStringList cookies);
    QString combine_cookies2(QStringList cookies);
    int get_url(QString url, QByteArray & html,  QString refer = ""); // this may using cookie, it in cookies member

public:
    int siteId;
    QString site_name;
    QString site_icon;
    QString home_page;
    QString encoding;
    QString host;
    short port;
    QString username_field_name;
    QString password_field_name;
    QString login_url;
    QString data;
    QString cookies;
    QString username;
    QString password;
    QString feed_url;
    QByteArray feed_page_html; // 
    QByteArray feed_page_utf8_html; // 
    QVector<QPair<QString, QString> > other_fields;
    int    step;
    int    next;  // 0, 1
    //char **letter_urls;
    QStringList letter_urls;
    int  (*letter_parsers[10])();
    int login_status ;
    QString error_message;
    QString friend_url;

    QString userid;
    QString nickname;
    QString signtext;
    QString photourl;
    QString activetext;
    QVector<QString> sysnotes;//depcreated
    //QVector<QByteArray> friend_list_page_html;
    //QVector<QByteArray> friend_list_page_utf8_html;
    QByteArray  friend_page_html;
    QByteArray  friend_page_utf8_html;

public:
    QTextCodec * codec;
    QTextCodec * u8codec;
};

#ifdef Q_WS_WIN
#define MY_EXPORT __declspec(dllexport)
#else
#define MY_EXPORT
#endif


#endif

