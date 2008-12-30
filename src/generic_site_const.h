#ifndef GENERIC_SITE_CONST_H
#define GENERIC_SITE_CONST_H

typedef bool (SiteConst::*SC_LETTER_PARSER_TYPE)(Account *, int);

class SiteConst : public QObject
{
    Q_OBJECT;
public:
    SiteConst(QObject *parent = 0);
    ~SiteConst();

    site_const_t * (*get_site_const)(int);

    SiteConst *dup();
    QString  homeUri();
    QString loginUri();
    QString feedUri();
    QString rewriteLetterLink(QString link);

    /* static void get_node_text(TidyDoc tdoc, TidyNode node, TidyBuffer *buf); */
    /* static void get_node_text2(TidyDoc tdoc, TidyNode node, TidyBuffer *buf, TidyBuffer *dbuf); */
    /* //reserve a element */
    /* static void get_node_text3(TidyDoc tdoc, TidyNode node, TidyBuffer *buf, const char *date_class, TidyBuffer *dbuf); */
    /* static QString get_html_head_css(TidyDoc tdoc, TidyBuffer *buf); */
    /* static QString get_html_head_js(TidyDoc tdoc, TidyBuffer *buf); */
    /* static QString get_html_head_all(TidyDoc tdoc, TidyBuffer buf); */

    QString rewriteRelativeLink(QString html, QString linkPrefix);
    QString rewriteRelativeLinkEx(QString html, QString linkBase);
    QString detectHtmlEncoding(TidyDoc tdoc);
    QString detectHtmlEncoding(const QString & htmlStr);

    /* bool parseHaineiLoginResponse(const QHttpResponseHeader & header, const QString & response, Account *accout); */
    /* bool parseXiaoneiLoginResponse(const QHttpResponseHeader &header, const QString &response, Account *account); */
    /* bool parseKaixinLoginResponse(const QHttpResponseHeader &header, const QString &response, Account *account); */
    /* bool parseUCHomeLoginResponse(const QHttpResponseHeader &header, const QString &response, Account *account); */
    /* //below for has check code  */
    /* bool parseUCHomeLoginResponseEx(const QHttpResponseHeader &header, const QString &response, Account *account); */
    /* bool parseHaineiFetchFeedResponse(Account *account); */
    /* bool parseXiaoneiFetchFeedResponse(Account *account); */
    /* bool parseKaixinFetchFeedResponse(Account *account); */
    /* bool parseUCHomeFetchFeedResponse(Account *account); */
    /* bool parseUCHomeResolveSecureCodeResponse(Account *account); */
    /* //bool parseHaineiLoginResponse(QString response); */

    /* bool parseUnimpledFetchLetterResponse(Account *account, int letterType); */

    /* bool parseHaineiFetchLetterResponse(Account *account, int letterType); */
    /* bool parseHaineiNoticeFetchLetterResponse(Account *account, int letterType); */
    
    /* bool parseKaixinFetchLetterResponse(Account *account, int letterType); */
    /* bool parseKaixinFetchPubMsgLetterResponse(Account *account, int letterType); */

    /* bool parseXiaoneiFetchLetterResponse(Account *account, int letterType); */

    /* bool parseUCHomeFetchLetterResponse(Account *account, int letterType); */
    /* bool parseUCHomeNoticeFetchLetterResponse(Account *account, int letterType); */

    char *htmlToUTF8(Account *account);//
    char *htmlToUTF8(Account *account, int letterType);//
    TidyDoc createAndCleanTidyDoc(const char *html);
    bool    cleanupTidyDoc(TidyDoc tdoc);
    TidyNode searchNode(TidyDoc tdoc, TidyNode parentNode, const char * class_name, int label);
    TidyNode searchParentNode(TidyDoc tdoc, TidyNode childNode, const char * class_name, int label);
    TidyNode searchNext(TidyDoc tdoc, TidyNode node);

public:
    enum {SITE_MIN, SITE_HAINEI = 0x01, SITE_XIAONEI = 0x02, SITE_KAIXIN = 0x04, 
          SITE_UCHOME = 0x08, SITE_MAX};
    //通知 的所有类型代码    
    enum {LET_MIN, LET_NEWBOX, LET_INBOX, LET_OUTBOX, LET_PUBMSG, LET_APPMSG, LET_COMMENT, LET_COMMENT_REPLY,
          LET_BBS, LET_BBS_REPLY, LET_NOTICE, LET_MAX};

    void * dlhandle;
    site_const_t * site_const;
    int siteId;

    //depcreated
    /* QString siteName; */
    /* QString siteEncoding; */
    /* QString siteIcon; */
    /* QString homePage; */
    /* QString loginUrl; */
    /* QString loginUserFieldName; */
    /* QString loginPasswordFieldName; */
    /* QString loginAutoLoginFieldName; */
    /* QString loginAutoLoginFieldValue; */
    /* QMap<QString, QString> loginOtherValues; */
    /* //typedef bool (SiteConst::*SC_LETTER_PARSER_TYPE)(Account *, int); */
    /* QMap<int, QPair<QString, SC_LETTER_PARSER_TYPE> > letterUrls; */

    /* bool    hasCheckCode; */

    /* QString feedUrl; */
    /* QString successSignature; */
    /* QString faildSignature; */
    /* QString errorMessageBegin; */
    /* QString errorMessageEnd; */
    /* QString feedContentBegin; */
    /* QString feedContentEnd; */

    /* QVector<QString> feedFields; */
    /* QVector<QString> feedSelector; */

    //QString cookieString;

    QTextCodec *codec;

    /* bool (SiteConst::*parseLogin)(const QHttpResponseHeader &, const QString &, Account *); */
    /* bool (SiteConst::*parseFetchFeed)(Account *); */
    /* bool (SiteConst::*parseFetchLetter)(Account *, int); */

};


#endif
