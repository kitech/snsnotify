#ifndef UCH_MAIN_FEED_H
#define UCH_MAIN_FEED_H

#include <QtCore>
#include <QtGui>
#include <QtWebKit>
#include <QtNetwork>

#include "feed_record.h"

/////////define
class UCHome_Main_SiteConst : public SiteConst
{
    Q_OBJECT;
public:
    UCHome_Main_SiteConst();
    ~UCHome_Main_SiteConst();

    //interface
    virtual int get_version();
    virtual QString parse_userid(TidyDoc doc);

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

    virtual QString avataUrl(QString uid, int type) ;
    virtual QString homePageUrl(QString uid);
    virtual bool sendMsg(QString uid, QString msg);

public slots:    
    virtual int login(Account *acc) ;
    virtual int logout() ;
    virtual int fetchFeedData() ;
    virtual int fetchFriendData();


    ////////private
private:
    void clean_my_name_lable();
    QString get_time_string(TidyDoc doc, TidyNode tnode);
    QString find_nick_name(TidyDoc doc);
    QString find_sign_text(TidyDoc doc);
    QString find_photo_url(TidyDoc doc);
    QString find_active_text(TidyDoc doc);
    QVector<QString> find_sys_notice(TidyDoc doc);
    QString find_sys_notice_by_type(TidyDoc doc, char *type_class);
    QString rewrite_relative_link(QString src);
    QString rewrite_relative_img(QString src);
    QString rewrite_relative_link_ex(QString src);
    QByteArray save_node_with_absolute_link(TidyDoc doc, TidyNode node);
    QByteArray save_node_with_absolute_link_and_filter(TidyDoc doc, TidyNode node, int tagid, char *class_or_id);
    int   uch_question_caculor(QString q);

private:
    //QWebView * webview;
    QWebPage * webpage;
    QWebFrame *webframe;
    QNetworkAccessManager * nam;
    QString login_step1_url;
    QString login_step2_url;
    QString login_step3_url;
    QString sess_string;
    QQueue<QVector<QString> > msgQueue;
    QBitArray   isLoging;
    unsigned char      loginRetryTimes;

private slots:
    void wkLoadStarted();
    void wkLoadFinished ( bool ok );
    void wkLoadProgress ( int progress );
};

#endif
