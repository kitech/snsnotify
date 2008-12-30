
#include "generic_site_const.h"



/////////////
static QString getXiaoneiFeedTypeImageName(QString imgUrl)
{
    QStringList sl = imgUrl.trimmed().split(QChar('/'));
    return sl.at(sl.count() - 1);
}


///////////////
//////////////
SiteConst::SiteConst(QObject *parent)
    : QObject(parent)
{
    dlhandle = NULL;
    get_site_const = NULL;
    site_const = NULL;
    
    //this->hasCheckCode = false;
    this->codec = NULL;
    //this->parseLogin = NULL;
}
SiteConst::~SiteConst()
{
    
}
SiteConst *SiteConst::dup()
{
    SiteConst *nsc = NULL;
    SiteConst *sc = this;

    nsc = new SiteConst(this->parent());
    nsc->siteId = sc->siteId;
    // nsc->siteName = sc->siteName;
    // nsc->siteEncoding = sc->siteEncoding;
    // nsc->siteIcon = sc->siteIcon;
    // nsc->homePage = sc->homePage;
    // nsc->loginUrl = sc->loginUrl;
    // nsc->loginUserFieldName = sc->loginUserFieldName;
    // nsc->loginPasswordFieldName = sc->loginPasswordFieldName;
    // nsc->loginAutoLoginFieldName = sc->loginAutoLoginFieldName;
    // nsc->loginAutoLoginFieldValue = sc->loginAutoLoginFieldValue;
    // nsc->loginOtherValues = sc->loginOtherValues;
    // nsc->letterUrls = sc->letterUrls;

    // nsc->hasCheckCode = sc->hasCheckCode;

    // nsc->feedUrl = sc->feedUrl;
    // nsc->successSignature = sc->successSignature;
    // nsc->faildSignature = sc->faildSignature;
    // nsc->errorMessageBegin = sc->errorMessageBegin;
    // nsc->errorMessageEnd = sc->errorMessageEnd;
    // nsc->feedContentBegin = sc->feedContentBegin;
    // nsc->feedContentEnd = sc->feedContentEnd;

    // nsc->feedFields = sc->feedFields;
    // nsc->feedSelector = sc->feedSelector;

    // //nsc->cookieString = sc->cookieString;

    // nsc->codec = sc->codec;

    // nsc->parseLogin = NULL;
    // nsc->parseFetchFeed = NULL;
    // nsc->parseLogin = sc->parseLogin;
    // nsc->parseFetchFeed = sc->parseFetchFeed;

    return nsc;
}

QString  SiteConst::homeUri()
{
    //return this->homePage;
    return QString(this->site_const->home_page);
}
QString SiteConst::loginUri()
{
    // if(this->loginUrl.startsWith("http")) 
    //     return this->loginUrl;
    // else {
    //     if(this->homePage.endsWith("/")) {
    //         return this->homePage.left(this->homePage.length()-1) + this->loginUrl;
    //     }else{
    //         return this->homePage + this->loginUrl;
    //     }
    // }
    QString loginUrl = QString(this->site_const->login_url);
    QString homePage = this->homeUri();
    if(loginUrl.startsWith("http")) 
        return loginUrl;
    else {
        if(homePage.endsWith("/")) {
            return homePage.left(homePage.length()-1) + loginUrl;
        }else{
            return homePage + loginUrl;
        }
    }    
}
QString SiteConst::feedUri()
{
    // if(this->feedUrl.startsWith("http"))
    //     return this->feedUrl;
    // else{
    //     if(this->homePage.endsWith("/")) {
    //         return this->homePage.left(this->homePage.length()-1) + this->feedUrl;
    //     }else{
    //         return this->homePage + this->feedUrl;
    //     }
    // }
}

QString SiteConst::rewriteLetterLink(QString link)
{
    // if(link.startsWith("http"))
    //     return link;
    // else{
    //     if(this->homePage.endsWith("/")) {
    //         return this->homePage.left(this->homePage.length()-1) + link;
    //     }else{
    //         return this->homePage + link;
    //     }
    // }    
}

QString SiteConst::rewriteRelativeLink(QString html, QString linkPrefix)
{
    int pos = 0;
    QString tmp = html;
    QString href = "href=\"";
    int len = 6;
    // tmp = tmp.replace("\n", "");
    // while(true) {
    //     pos = tmp.indexOf(href, pos, Qt::CaseInsensitive);
    //     if(pos == -1) break;
    //     if(tmp.mid(pos+len, 4) == "http") {
    //         pos += len;
    //     }else if(tmp.at(pos+len) == QChar('#')) {
    //         tmp = tmp.insert(pos+len, linkPrefix);
    //     }else{
    //         if(tmp.at(pos+len) == QChar('/') && linkPrefix.endsWith("/")) {
    //             tmp = tmp.insert(pos+len, linkPrefix.left(linkPrefix.length()-1));
    //         }else{
    //             tmp = tmp.insert(pos+len, linkPrefix);
    //         }
    //         pos += len;
    //     }
    // }
    // //q_debug()<<tmp<<"\n";
    // return tmp;
}

QString SiteConst::rewriteRelativeLinkEx(QString html, QString linkBase)
{
    int pos = 0;
    QString tmp = html;
    // QString str1;
    // QString href;
    // QRegExp arex( "(href[ ]*=[ ]*\")(.*)\"");
    // arex.setMinimal(true);
    // QString linkHome = linkBase.left(linkBase.length() - QUrl(linkBase).path().length());
    // QString linkPath = linkBase.left(linkBase.lastIndexOf("/")+1);
    // q_debug()<<linkHome<<linkPath;

    // tmp = tmp.replace("\n", "");
    // while(true) {
    //     //pos = tmp.indexOf(href, pos, Qt::CaseInsensitive);
    //     pos = arex.indexIn(tmp, pos);
    //     if(pos == -1) break;
    //     str1 = arex.cap(1);
    //     href = arex.cap(2);
    //     if(href.startsWith("http")) {
    //         pos += arex.matchedLength();
    //     }else if(href.startsWith("javascript:", Qt::CaseInsensitive)) {
    //         pos += arex.matchedLength();
    //     }else if(href.startsWith(QChar('/'))) {
    //         tmp = tmp.insert(pos+str1.length(), linkHome);
    //         pos += arex.matchedLength() + linkHome.length();
    //     }else if(href.startsWith(QChar('#'))) {
    //         tmp = tmp.insert(pos+str1.length(), linkBase);
    //         pos += arex.matchedLength() + linkBase.length();
    //     }else{
    //         tmp = tmp.insert(pos+str1.length(), linkPath);
    //         pos += arex.matchedLength() + linkPath.length();
    //     }
    // }
    //q_debug()<<tmp<<"\n";
    return tmp;
}


char *SiteConst::htmlToUTF8(Account *account)
{
    char *input = NULL;
    QTextCodec *aimCodec = NULL;
    QTextCodec *srcCodec = NULL;
    QString  html;
    QString  charset;

    aimCodec = so->codec;
    srcCodec = account->sc->codec;

    charset = this->detectHtmlEncoding(account->loginOutput);
    q_debug()<<aimCodec<<srcCodec<<aimCodec->name()<<srcCodec->name()<<charset;
    if(aimCodec->name() == srcCodec->name()) {
        html = account->loginOutput;
    }else{
        QString tmp = srcCodec->toUnicode(account->loginOutput.toAscii());
        html = aimCodec->fromUnicode(tmp);
    }
    input = strdup(html.toAscii().data());

    return input;    
}

char *SiteConst::htmlToUTF8(Account *account, int letterType)
{
    char *input = NULL;
    QTextCodec *aimCodec = NULL;
    QTextCodec *srcCodec = NULL;
    QString  html;
    QString  charset;
    QString  srcHtml;

    aimCodec = so->codec;
    srcCodec = account->sc->codec;
    srcHtml = account->letOutputs.value(letterType);

    charset = this->detectHtmlEncoding(srcHtml);
    q_debug()<<aimCodec<<srcCodec<<aimCodec->name()<<srcCodec->name()<<charset;
    if(aimCodec->name() == srcCodec->name()) {
        html = srcHtml;
    }else{
        QString tmp = srcCodec->toUnicode(srcHtml.toAscii());
        html = aimCodec->fromUnicode(tmp);
    }
    input = strdup(html.toAscii().data());

    return input;    
}


QString SiteConst::detectHtmlEncoding(TidyDoc tdoc)
{
    QString charset;
    TidyNode  head = NULL;
    TidyNode  meta = NULL;
    TidyAttr  attr = NULL;
    const char * value = NULL;
    const char * ptr = NULL;
	const char * str = "charset=";

    head = tidyGetHead(tdoc);
    if(head != NULL) {
        for(meta = tidyGetChild(head); meta; meta = tidyGetNext(meta)) {
            if(tidyNodeGetId(meta) == TidyTag_META) {
                //attr = tidyAttrGetById(meta, TidyAttr_CONTENT);
                attr = tidyAttrGetCONTENT(meta);
                if(attr != NULL) {
                    value = tidyAttrValue(attr);
                    if(value != NULL) {
						if(QString(value).indexOf(str) != -1) {
                            charset = QString(value).right(QString(value).length() - QString(value).indexOf(str)-8).trimmed().toAscii().data();
                            break;
                        }
                    }
                }
            }
        }
    }
    return charset;
}
QString SiteConst::detectHtmlEncoding(const QString & htmlStr)
{
    QString charset;

    int pos = htmlStr.indexOf("charset=", 0, Qt::CaseInsensitive);
    for(int i = pos + 8 ; ; i ++) {
        if(htmlStr.at(i) == QChar('"') || htmlStr.at(i) == QChar('>')) break;
        charset += htmlStr.at(i);
    }

    return charset;
}

TidyDoc SiteConst::createAndCleanTidyDoc(const char *html)
{
    TidyDoc tdoc = NULL;
    TidyBuffer *errorBuffer;
    Bool ok;
    int rc = -1;
    const char *input = html;
    QString charset ;

    Q_ASSERT(html != NULL);
    Q_ASSERT(strlen(html) > 0);

    errorBuffer = (TidyBuffer*)malloc(sizeof(TidyBuffer));
    tidyBufInit(errorBuffer);
    tdoc = tidyCreate();
#ifdef WIN32
    tidySetAppData(tdoc, errorBuffer);
#else
    //use a c++ feather typeof, g++ is ok, but i dont know is vc 2005 is support
    tidySetAppData(tdoc, (typeof(tidyGetAppData(tdoc)))errorBuffer); //  maybe use tidylib's dynamic and static case the differ
#endif
    tidySetCharEncoding(tdoc, "utf8");
    tidyOptSetInt( tdoc, TidyIndentContent, TidyAutoState );
    if ( tidyOptGetInt(tdoc, TidyIndentSpaces) == 0) {
        tidyOptResetToDefault(tdoc, TidyIndentSpaces);
    }
    ok = tidyOptSetBool(tdoc, TidyXhtmlOut, yes);
    if(ok) {
        rc = tidySetErrorBuffer(tdoc, errorBuffer);
    }else{
        q_debug()<<"set out error"<<ok;
        return tdoc;
    }
    if(rc >= 0) {
        rc = tidyParseString(tdoc, input);
    }else{
        q_debug()<<"set err buf error"<<rc;
        return tdoc;
    }

    charset = this->detectHtmlEncoding(tdoc);

    if(rc >= 0) {
        rc = tidyCleanAndRepair(tdoc);
    }else{
        q_debug()<<"parse error";
        return tdoc;
    }
    if(rc >= 0) {
        rc = tidyRunDiagnostics(tdoc);
    }else{
        q_debug()<<"clean error";
    }

    q_debug()<<"HTML charset:"<<charset
             <<"tidyErrorCount :"<<tidyErrorCount(tdoc)
             <<"tidyWarningCount :"<<tidyWarningCount(tdoc);

    return tdoc;
}
bool SiteConst::cleanupTidyDoc(TidyDoc tdoc)
{
    Q_ASSERT(tdoc != NULL);
    TidyBuffer *errbuf = NULL;

    errbuf = (TidyBuffer*)tidyGetAppData(tdoc);
    Q_ASSERT(errbuf != NULL);

    tidyBufFree(errbuf);
    tidyRelease(tdoc);

    return true;
}
/*
  if parentNode == NULL, search start from body node
  if class_name == NULL , omit the label's attr
  if class_name != NULL, then class_name's length must > 0
  label is TidyTag_XXX
 */
TidyNode SiteConst::searchNode(TidyDoc tdoc, TidyNode parentNode, const char * class_name, int label)
{
    TidyNode tnode = NULL;
    TidyNode currNode = NULL;
    TidyNode childNode = NULL;
    TidyAttr attr = NULL;
    ctmbstr value = NULL;

    Q_ASSERT(tdoc != NULL);
    if(class_name != NULL) {
        Q_ASSERT(strlen(class_name) > 0);
    }

    if(parentNode == NULL) {
        currNode = tidyGetBody(tdoc);
        Q_ASSERT(currNode != NULL);
    }else{
        currNode = parentNode;
    }

    for(childNode = tidyGetChild(currNode); childNode ; childNode = tidyGetNext(childNode)) {
        if(tidyNodeGetId(childNode) == label) {
            if(class_name == NULL) {
                return childNode;
            }
            for(attr = tidyAttrFirst(childNode); attr; attr = tidyAttrNext(attr)) {
                if(tidyAttrGetId(attr) == TidyAttr_CLASS
                   ||
                   tidyAttrGetId(attr) == TidyAttr_ID) {
                    value = tidyAttrValue(attr);
                    if(strcasecmp(value, class_name) == 0) {
                        return childNode;
                    }
                    value = NULL;
                }
            }
        }
    }
    for(childNode = tidyGetChild(currNode); childNode; childNode = tidyGetNext(childNode)) {
        tnode = searchNode(tdoc, childNode, class_name, label);
        if(tnode != NULL) {
            break;
        }
    }    

    return tnode;
}

TidyNode SiteConst::searchParentNode(TidyDoc tdoc, TidyNode childNode, const char * class_name, int label)
{
    TidyNode tnode = NULL;    
    TidyNode parentNode = NULL;
    TidyAttr attr = NULL;
    ctmbstr value = NULL;

    Q_ASSERT(childNode != NULL);
    Q_ASSERT(class_name != NULL);
    Q_ASSERT(strlen(class_name) > 0);
    
    for(parentNode = tidyGetParent(childNode); parentNode; parentNode = tidyGetParent(parentNode)) {
        if(tidyNodeGetId(parentNode) == label) {
            for(attr = tidyAttrFirst(parentNode); attr; attr = tidyAttrNext(attr)) {
                if(tidyAttrGetId(attr) == TidyAttr_CLASS
                   ||
                   tidyAttrGetId(attr) == TidyAttr_ID) {
                    value = tidyAttrValue(attr);
                    if(strcasecmp(value, class_name) == 0) {
                        return parentNode;
                    }
                    value = NULL;
                }
            }
        }        
    }

    return tnode;
}

TidyNode SiteConst::searchNext(TidyDoc tdoc, TidyNode node)
{
    TidyNode tnode = NULL;
    TidyNode nnode = NULL;
    TidyAttr attr = NULL;
    ctmbstr  class_name = NULL;
    ctmbstr  curr_class_name = NULL;
    
    Q_ASSERT(tdoc != NULL);
    Q_ASSERT(node != NULL);

    for(attr = tidyAttrFirst(node); attr ; attr= tidyAttrNext(attr)) {
        if(tidyAttrGetId(attr) == TidyAttr_CLASS) {
            class_name = tidyAttrValue(attr);
            break;
        }
    }
    Q_ASSERT(class_name != NULL);

    for(tnode = tidyGetNext(node); tnode; tnode = tidyGetNext(tnode)) {
        for(attr = tidyAttrFirst(tnode); attr ; attr= tidyAttrNext(attr)) {
            if(tidyAttrGetId(attr) == TidyAttr_CLASS) {
                curr_class_name = tidyAttrValue(attr);
                if(strcasecmp(curr_class_name, class_name) ==0 ) {
                    return tnode;
                }
            }
        }        
    }

    return NULL;
}

// void SiteConst::get_node_text(TidyDoc tdoc, TidyNode node, TidyBuffer *buf)
// {
//     TidyBuffer text = {0};    
//     tidyBufInit(&text);

//     switch(tidyNodeGetType(node)) {
//     case TidyNode_Text:
//         //q_debug()<<"text found";
//         tidyNodeGetText(tdoc, node, &text);
//         if(text.bp[text.size-1] == '\n') {
//             text.size -= 1;
//         }
//         tidyBufAppend(buf, text.bp, text.size);
//         break;
//     default:
//         switch(tidyNodeGetId(node)) {
//         case TidyTag_A:
//             //q_debug()<<"a node found";
//             get_node_text(tdoc, tidyGetChild(node), buf);        
//             break;
//         case TidyTag_P:
//         case TidyTag_SPAN:
//             get_node_text(tdoc, tidyGetChild(node), buf);
//             break;
//         case TidyTag_Q:
//             get_node_text(tdoc, tidyGetChild(node), buf);
//             break;
//         case TidyTag_PLAINTEXT:
//             //q_debug()<<"plain text found";
//             tidyNodeGetText(tdoc, node, &text);
//             tidyBufAppend(buf, text.bp, text.size);
//             break;
//         case TidyTag_H4:
//         case TidyTag_H6:
//         case TidyTag_DIV:
//             TidyNode child;
//             for(child = tidyGetChild(node) ; child ; child = tidyGetNext(child)) {
//                 get_node_text(tdoc, child, buf);
//             }
//             break;
//         case TidyTag_IMG:
//             //q_debug()<<"it is img";
//             break;
//         default:
//             q_debug()<<"what is it?";
//             break;
//         }
//         break;
//     };
//     tidyBufFree(&text);
// }
// void SiteConst::get_node_text3(TidyDoc tdoc, TidyNode node, TidyBuffer *buf, const char *date_class, TidyBuffer *dbuf)
// {
//     TidyBuffer text = {0};    
//     TidyNode child;
//     TidyAttr  attr;
//     ctmbstr ptr = NULL;

//     tidyBufInit(&text);    
//     switch(tidyNodeGetType(node)) {
//     case TidyNode_Text:
//         //q_debug()<<"text found";
//         tidyNodeGetText(tdoc, node, &text);
//         if(text.bp[text.size-1] == '\n') {
//             text.size -= 1;
//         }
//         tidyBufAppend(buf, text.bp, text.size);
//         break;
//     default:
//         switch(tidyNodeGetId(node)) {
//         case TidyTag_A:
//             //if javascript , omit link, only text
//             //attr = tidyAttrGetById(node, TidyAttr_HREF);
//             attr = tidyAttrGetHREF(node);
//             if(attr != NULL) {
//                 ptr = tidyAttrValue(attr);
//                 if(ptr != NULL) {
//                     if(strlen(ptr) >= 10 && strncasecmp(ptr, "javascript", 10) == 0) {
//                         get_node_text3(tdoc, tidyGetChild(node), buf, date_class, dbuf);
//                         break;
//                     }
//                     if(ptr[0] == '#') { //  anchor  tag
//                         get_node_text3(tdoc, tidyGetChild(node), buf, date_class, dbuf);
//                         break;
//                     }
//                 }
//             }
//             attr = NULL; ptr = NULL;
//             tidyBufClear(&text);
//             tidyNodeGetText(tdoc, node, &text);
//             tidyBufAppend(buf, text.bp, text.size);            
//             tidyBufClear(&text);
//             break;
//         case TidyTag_SPAN:
//             //q_debug()<<"a node found";
//             attr = tidyAttrFirst(node);
//             if(attr && tidyAttrGetId(attr) == TidyAttr_CLASS) {
//                 if(strcasecmp(tidyAttrValue(attr), date_class) == 0) {
//                     //q_debug()<<" got a date";
//                     if(dbuf->size != 0 ) {
//                         //Q_ASSERT(dbuf->size == 0);
//                         tidyBufClear(dbuf);
//                     }
//                     tidyNodeGetText(tdoc, tidyGetChild(node), dbuf);
//                     break;
//                 }
//             }
//             goto get_all_child_text;
//             break;
//         case TidyTag_PLAINTEXT:
//             //q_debug()<<"plain text found";
//             tidyNodeGetText(tdoc, node, &text);
//             tidyBufAppend(buf, text.bp, text.size);
//             break;
//         case TidyTag_P:
//             if(buf->size > 0) {
//                 tidyBufAppend(buf, (void*)"<BR>", 4);
//             }
//         case TidyTag_H2:
//         case TidyTag_H3:
//         case TidyTag_H6:
//         //case TidyTag_A:
//         case TidyTag_Q:
//         case TidyTag_UL:
//         case TidyTag_LI:
//         case TidyTag_DIV:
//         get_all_child_text:
//             for(child = tidyGetChild(node) ; child ; child = tidyGetNext(child)) {
//                 get_node_text3(tdoc, child, buf, date_class, dbuf);
//             }
//             break;
//         default:
//             break;
//         }
//         break;
//     };
//     tidyBufFree(&text);
    
// }

// QString SiteConst::get_html_head_css(TidyDoc tdoc, TidyBuffer *buf)
// {
//     Q_ASSERT(tdoc != NULL);
//     Q_ASSERT(buf != NULL);

//     QString header;
//     TidyNode head = NULL;
//     TidyNode node = NULL;
//     TidyAttr attr = NULL;
//     TidyBuffer tbuf = {0};
//     tidyBufInit(&tbuf);
//     ctmbstr  name = NULL;

//     tidyBufClear(buf);
//     head = tidyGetHead(tdoc);
//     if(head != NULL) {
//         for(node = tidyGetChild(head); node != NULL; node = tidyGetNext(node)) {
//             name = tidyNodeGetName(node);
//             if(name != NULL && strcasecmp(name, "link") == 0) {
//                 for(attr = tidyAttrFirst(node); attr != NULL; attr = tidyAttrNext(attr)) {
//                     if(tidyAttrGetId(attr) == TidyAttr_TYPE
//                        && strcasecmp(tidyAttrValue(attr), "text/css") == 0 ) {
//                         tidyBufClear(&tbuf);
//                         tidyNodeGetText(tdoc, node, &tbuf);
//                         tidyBufAppend(buf, tbuf.bp, tbuf.size);
//                     }
//                 }
//             }
//         }
//     }
//     tidyBufFree(&tbuf);
    
//     header = (char*)(buf->bp);
    
//     return header;
// }

// QString SiteConst::get_html_head_js(TidyDoc tdoc, TidyBuffer *buf)
// {
//     return QString();
// }

// QString SiteConst::get_html_head_all(TidyDoc tdoc, TidyBuffer buf)
// {
//     return QString();
// }


// ///////////////////////
// bool SiteConst::parseHaineiLoginResponse(const QHttpResponseHeader &header, const QString &response, Account *account)
// {
//     qDebug()<<"login result parser for hainei";

//     bool ok = false;
//     account->cookies.clear();
//     QString encodedUserName = QUrl::toPercentEncoding(account->userName);
//     QList<QPair<QString, QString> > values = header.values();
//     for(int i = 0; i < values.count() ; i++) {
//         if(values.at(i).first == QString("Set-Cookie")) {
//             account->cookies.append(QPair<QString, QString>("Cookie", values.at(i).second));
//             if(values.at(i).second.left(2 + encodedUserName.length()) == QString("m=%1").arg(encodedUserName)) {
//                 qDebug()<<"login success";
//                 account->loginStatus = Account::LOG_LOGGED;
//                 //return true;
//                 ok = true;
//             }
//         }
//     }    
//     if(ok) return ok;

//     account->loginStatus = Account::LOG_ERROR;
//     //search error msg
//     if(header.contentLength() == 0 && response.length() == 0) {
//         account->loginErrorMessage = tr("Unknown error1");
//         return false;
//     }
//     QString errdiv = account->sc->errorMessageBegin;
//     int empos = response.indexOf(errdiv);
//     if(empos == -1) {
//         account->loginErrorMessage = tr("Unknown error2");
//         return false;
//     }
//     QString errend = account->sc->errorMessageEnd;
//     int emend = response.indexOf(errend, empos);
//     if(emend == -1) {
//         account->loginErrorMessage = tr("Unknown error3");
//         return false;        
//     }
    
//     account->loginErrorMessage = response.mid(empos + errdiv.length(), emend - empos - errdiv.length());

//     return false;
// }

// bool SiteConst::parseXiaoneiLoginResponse(const QHttpResponseHeader &header, const QString &response, Account *account)
// {
//     qDebug()<<"login result parser for xiaonei";

//     bool ok = false;
//     account->cookies.clear();
//     QString encodedUserName = QUrl::toPercentEncoding(account->userName);
//     QList<QPair<QString, QString> > values = header.values();
//     for(int i = 0; i < values.count() ; i++) {
//         if(values.at(i).first == QString("Set-Cookie")) {
//             if(values.at(i).second.indexOf("kl=null") == 0 ) {
//                 //drop the line
//             }else{
//                 account->cookies.append(QPair<QString, QString>("Cookie", values.at(i).second));
//             }
//             //account->cookies.append(QPair<QString, QString>("Cookie", values.at(i).second));
//             if(values.at(i).second.left(strlen("kl=")) == QString("kl=") 
//                && values.at(i).second.length() > (3 + 32 + 1 + 1)
//                && values.at(i).second.left(strlen("kl=")+4) != QString("kl=null") ) {
//                 qDebug()<<"login success";
//                 account->loginStatus = Account::LOG_LOGGED;
//                 //return true;
//                 ok = true;
//             }
//         }
//     }    
//     if(ok) return ok;

//     account->loginStatus = Account::LOG_ERROR;
//     //search error msg
//     if(header.contentLength() == 0 && response.length() == 0) {
//         account->loginErrorMessage = tr("Unknown error1");
//         return false;
//     }
//     QString errdiv = account->sc->errorMessageBegin;
//     int empos = response.indexOf(errdiv);
//     if(empos == -1) {
//         account->loginErrorMessage = tr("Unknown error2");
//         return false;
//     }
//     QString errend = account->sc->errorMessageEnd;
//     int emend = response.indexOf(errend, empos);
//     if(emend == -1) {
//         account->loginErrorMessage = tr("Unknown error3");
//         return false;        
//     }
    
//     account->loginErrorMessage = response.mid(empos + errdiv.length(), emend - empos - errdiv.length());
    

//     return false;
// }
// //because of vc 2005 utf8 problem , we add some space at end of utf8 chinese const
// static const char * hainei_feed_types[] = {
//     "应用  ", "http://xnimg.cn/img/newsfeed/developer.gif", // "http://static.hainei.com/img/appicons.gif",
//     "投票  ", "http://static.hainei.com/img/icon/ico-vote.gif",
//     "迷你博客  ", "http://static.hainei.com/img/miniblog-icon.gif",
//     "日志  ", "http://static.hainei.com/img/blog-icon.gif",
//     "相册  ", "http://static.hainei.com/img/photo-icon.gif",
//     "电影  ", "http://static.hainei.com/img/movie-icon.gif",
//     "校友录  ","http://static.hainei.com/img/class-icon.gif",
//     "扎绵羊  ","http://static.hainei.com/img/icon/ico-sheep.gif",
//     "群组  ", "http://static.hainei.com/icon/group.gif",
//     "分享  ", "http://static.hainei.com/icon/share.gif",
//     "相似度  ", "http://static.hainei.com/icon/similar.gif",
//     "电台  ", "http://static.hainei.com/icon/radio.gif",
//     "停车大战  ", "http://static.hainei.com/icon/parking.gif",
//     "买卖好友  ","http://static.hainei.com/icon/usersale.gif",
//     "手机  ", "http://static.hainei.com/icon/mobile.gif",
//     "记事本  ", "http://static.hainei.com/icon/notepad.gif",
//     "好友  ", "http://u.discuz.net/home/image/icon/friend.gif",
//     NULL
// };

// static const char * hainei_find_feed_icon_by_name(const char *name)
// {
//     int i = 0 ;
//     const char *mn = NULL;
//     const char *icon = NULL;

//     for(i = 0 ; ; i+= 2) {
//         mn = hainei_feed_types[i];
//         if(mn == NULL) break;
//         icon = hainei_feed_types[i+1];
//         //printf("%s=?%s\n", name, mn);
//         if(strncmp(name, mn, strlen(mn)-2) == 0) {
//             break;
//         }
//         icon = NULL;
//     }

//     return icon;
// }

// bool SiteConst::parseHaineiFetchFeedResponse(Account *account)
// {
//     qDebug()<<"fetch feed result parser for xiaonei";
    
//     char *input = NULL;
//     TidyDoc tdoc = NULL;
//     TidyNode anode = NULL;

//     input = this->htmlToUTF8(account);
//     if(input == NULL) {
//         return false;
//     }

//     tdoc = this->createAndCleanTidyDoc(input);
//     Q_ASSERT(tdoc != NULL);

//     //test module
//     int (*get_version)();
//     void *handle = dlopen("./modules/libhainei_feed.so", RTLD_LAZY);
//     if(!handle) {
//         q_debug()<<"open module error";
//         return false;
//     }
//     dlerror();
//     *(void **) (&get_version) = dlsym(handle, "version");
//     int ver = get_version();
//     q_debug()<<"test module ver:"<<ver;
//     dlclose(handle);
//     //test module end
//     return true;

//     anode = this->searchNode(tdoc, NULL, "brief", TidyTag_DIV);
//     if(anode == NULL) {
//         q_debug()<<"can not find feed node";
//         //TODO cleanup
//         return false;
//     }
//     anode = this->searchParentNode(tdoc, anode, "sect-content", TidyTag_DIV);
//     if(anode == NULL) {
//         q_debug()<<"can not find feed node";
//         //TODO cleanup
//         return false;
//     }

//     //parser body
//     QList<FeedRecord*> allFeeds;
//     QDateTime nowDate = QDateTime::currentDateTime();
//     QDateTime currDate = nowDate;
//     QString yesterday = "昨天";

//     TidyNode root = anode;
//     TidyNode ul;
//     TidyNode li;
//     ctmbstr name;
//     int iday = 0;
//     const char *icon = NULL;
//     q_debug()<<"begin parser feed content node"<<root;
//     for(ul = tidyGetChild(root); ul; ul = tidyGetNext(ul)) {
//         name = tidyNodeGetName(ul);
//         //qDebug()<<name;
//         if(tidyNodeGetId(ul) == TidyTag_H4) {
//             TidyBuffer tbuf = {0};
//             tidyNodeGetText(tdoc, tidyGetChild(ul), &tbuf);
//             QString ds = QString((char*)tbuf.bp).trimmed();
//             if(ds == yesterday) {
//                 iday = 1;
//                 currDate = nowDate.addDays(-1);
//             }else{
//                 QDate date = QDate::fromString(ds, "yyyy-MM-dd");
//                 currDate.setDate(date);
//                 iday = currDate.daysTo(nowDate);
//             }
//             tidyBufFree(&tbuf);
//         }else{
//             for(li = tidyGetChild(ul); li ; li = tidyGetNext(li)) {
//                 TidyNode feednode = tidyGetChild(li);
//                 TidyNode an = tidyGetChild(feednode);                
//                 TidyNode atn = tidyGetChild(an);
//                 TidyNode divn = tidyGetNext(an);
//                 TidyBuffer tbuf = {0};
//                 TidyBuffer dbuf = {0};
//                 tidyBufInit(&tbuf);
//                 tidyBufInit(&dbuf);
//                 QString text;
//                 FeedRecord *fr = new FeedRecord();
//                 fr->account = account;

//                 tidyNodeGetText(tdoc, atn, &tbuf);
//                 text = QString((char*)tbuf.bp).trimmed();
//                 text = so->codec->toUnicode(text.toAscii());
//                 //qDebug()<<"1\t"<<text;
//                 fr->type = text;

//                 this->get_node_text3(tdoc, divn, &tbuf, "vvvvvv", &dbuf);
//                 text = QString((char*)tbuf.bp).trimmed();
//                 text = so->codec->toUnicode(text.toAscii());
//                 fr->content = this->rewriteRelativeLink(text, account->sc->homeUri());
//                 fr->content = fr->content.right(fr->content.length() - fr->type.length());

//                 //search type's icon
//                 icon = hainei_find_feed_icon_by_name((char*)tbuf.bp);
//                 if(icon != NULL) {
//                     fr->trends = QString(icon);
//                     fr->type = getXiaoneiFeedTypeImageName(fr->trends);
//                 }

//                 fr->date = QDateTime(currDate);
//                 fr->day = iday;
//                 allFeeds.append(fr);
//                 //
//             }
//         }
//     }
//     qDebug()<<"have days: "<<iday;
//     account->tmpFeeds = allFeeds;

//     //cleanup
//     free(input);
//     this->cleanupTidyDoc(tdoc);
    
//     return true;
// }

// void SiteConst::get_node_text2(TidyDoc tdoc, TidyNode node, TidyBuffer *buf, TidyBuffer *dbuf)
// {
//     TidyBuffer text = {0};    
//     TidyNode child;
//     TidyAttr  attr;

//     tidyBufInit(&text);    
//     switch(tidyNodeGetType(node)) {
//     case TidyNode_Text:
//         //q_debug()<<"text found";
//         tidyNodeGetText(tdoc, node, &text);
//         if(text.bp[text.size-1] == '\n') {
//             text.size -= 1;
//         }
//         tidyBufAppend(buf, text.bp, text.size);
//         break;
//     default:
//         switch(tidyNodeGetId(node)) {
//         case TidyTag_SPAN:
//             //q_debug()<<"a node found";
//             attr = tidyAttrFirst(node);
//             if(attr && tidyAttrGetId(attr) == TidyAttr_CLASS) {
//                 if(strcasecmp(tidyAttrValue(attr), "date") == 0) {
//                     //q_debug()<<" got a date";
//                     if(dbuf->size != 0 ) {
//                         //Q_ASSERT(dbuf->size == 0);
//                         tidyBufClear(dbuf);
//                     }
//                     tidyNodeGetText(tdoc, tidyGetChild(node), dbuf);
//                     break;
//                 }
//             }
//         case TidyTag_H2:
//         case TidyTag_H3:
//         case TidyTag_A:
//             //q_debug()<<"a node found";
//             //get_node_text2(tdoc, tidyGetChild(node), buf);        
//             for(child = tidyGetChild(node) ; child ; child = tidyGetNext(child)) {
//                 get_node_text2(tdoc, child, buf, dbuf);
//             }
//             break;
//         case TidyTag_P:
//             get_node_text2(tdoc, tidyGetChild(node), buf, dbuf);
//             break;
//         case TidyTag_Q:
//             get_node_text2(tdoc, tidyGetChild(node), buf, dbuf);
//             break;
//         case TidyTag_PLAINTEXT:
//             //q_debug()<<"plain text found";
//             tidyNodeGetText(tdoc, node, &text);
//             tidyBufAppend(buf, text.bp, text.size);
//             break;
//         case TidyTag_H6:
//         case TidyTag_LI:
//         case TidyTag_DIV:
//             for(child = tidyGetChild(node) ; child ; child = tidyGetNext(child)) {
//                 get_node_text2(tdoc, child, buf, dbuf);
//             }
//             break;
//         default:
//             break;
//         }
//         break;
//     };
//     tidyBufFree(&text);
// }

// bool SiteConst::parseXiaoneiFetchFeedResponse(Account *account)
// {
//     qDebug()<<"fetch feed result parser for xiaonei";

//     char *input = NULL;
//     TidyDoc tdoc = NULL;
//     TidyNode anode = NULL;

//     input = this->htmlToUTF8(account);
//     if(input == NULL) {
//         return false;
//     }

//     tdoc = this->createAndCleanTidyDoc(input);
//     Q_ASSERT(tdoc != NULL);
    
//     anode = this->searchNode(tdoc, NULL, account->sc->feedContentBegin.toAscii().data() , TidyTag_DIV);
//     if(anode == NULL) {
//         q_debug()<<"can not find feed node";
//         //TODO cleanup
//         return false;
//     }

//     //parser body
//     QList<FeedRecord*> allFeeds;
//     QDateTime nowDate = QDateTime::currentDateTime();
//     QDateTime currDate = nowDate;
//     QString yesterday = "昨天";

//     TidyNode root = anode;
//     TidyNode ul;
//     TidyNode li;
//     ctmbstr name;
//     int iday = 0;

//     q_debug()<<"begin parser feed content node"<<root;
//     TidyNode img;
//     TidyNode hh;
//     TidyAttr attr;
//     FeedRecord *fr = NULL;
//     for(ul = tidyGetChild(root); ul; ul = tidyGetNext(ul)) {
//         name = tidyNodeGetName(ul);
//         //q_debug()<<name;
//         li = tidyGetChild(ul);
//         if(tidyNodeGetId(li) != TidyTag_DIV) {
//             li = tidyGetNext(li);
//         }
//         if(li == NULL) {
//             break;  // this account no feed
//         }
//         name = tidyNodeGetName(li);
//         //qDebug()<<"    "<<name;
//         hh = tidyGetChild(li);
//         if(tidyNodeGetId(hh) == TidyTag_A) {
//             img = tidyGetChild(hh);
//             Q_ASSERT(tidyNodeGetId(img) == TidyTag_IMG);            
//         }else{            
//             img = hh;
//             Q_ASSERT(tidyNodeGetId(img) == TidyTag_IMG);            
//         }
//         for(attr = tidyAttrFirst(img); attr ; attr = tidyAttrNext(attr)) {
//             if(tidyAttrGetId(attr) == TidyAttr_SRC) {
//                 name = tidyAttrValue(attr);
//                 //qDebug()<<"        "<<name;
//                 break;
//             }
//         }

//         //fill feedRecord
//         fr = new FeedRecord();
//         fr->account = account;
//         fr->type = getXiaoneiFeedTypeImageName(QString(name));
//         fr->trends = QString(name);

//         //title
//         TidyBuffer tbuf = {0};
//         TidyBuffer dbuf = {0};
//         tidyBufInit(&tbuf);
//         tidyBufInit(&dbuf);
//         // if(tidyGetNext(hh))
//         //     get_node_text2(tdoc, tidyGetNext(hh), &tbuf, &dbuf);
//         // li = tidyGetNext(li);
//         // Q_ASSERT(tidyNodeGetId(li) == TidyTag_DIV);
//         // get_node_text2(tdoc, li, &tbuf, &dbuf);
//         get_node_text3(tdoc, tidyGetNext(li), &tbuf, "date", &dbuf);
//         QString msg = QString((char*)tbuf.bp).trimmed();
//         if(msg.left(6) == "&nbsp;") 
//             msg = msg.right(msg.length() - 6);
//         if(msg.left(strlen("设置")) == "设置") 
//             msg = msg.right(msg.length() - strlen("设置"));

//         msg = codec->toUnicode(msg.toAscii());
//         QString datetime = QString((char*)dbuf.bp).trimmed();
//         //qDebug()<<"        "<<msg<<" ("<< datetime <<")";
//         if(datetime.length() == 5) {
//             QTime currTime = QTime::fromString(datetime, "hh:mm");
//             currDate.setTime(currTime);
//         }else if(datetime.length() == 14) {
//             currDate = QDateTime::fromString(QString("20") + datetime, "yyyy-MM-dd hh:mm");
//         }else if(datetime.length() == 0) {
//         }else {
//             Q_ASSERT(1 == 2);
//         }
//         //qDebug()<<currDate;
//         iday = currDate.daysTo(nowDate);

//         //fill feedRecord
//         fr->content = msg;
//         fr->day = iday;
//         fr->date = currDate;

//         tidyBufFree(&tbuf);
//         tidyBufFree(&tbuf);

//         allFeeds.append(fr);
//     }
//     qDebug()<<"have days: "<<iday;
//     account->tmpFeeds = allFeeds;

//     //cleanup
//     free(input);
//     this->cleanupTidyDoc(tdoc);
    
//     return true;
// }
// bool SiteConst::parseKaixinLoginResponse(const QHttpResponseHeader &header, const QString &response, Account *account)
// {
//     qDebug()<<"login result parser for kaixin";    
//     bool ok = false;
//     account->cookies.clear();
//     QString encodedUserName = QUrl::toPercentEncoding(account->userName);
//     QList<QPair<QString, QString> > values = header.values();
//     for(int i = 0; i < values.count() ; i++) {
//         if(values.at(i).first == QString("Set-Cookie")) {
//             account->cookies.append(QPair<QString, QString>("Cookie", values.at(i).second));
//             if(values.at(i).second.left(7 + encodedUserName.length()) == QString("_email=%1").arg(encodedUserName)) {
//                 qDebug()<<"login success";
//                 account->loginStatus = Account::LOG_LOGGED;
//                 //return true;
//                 ok = true;
//             }
//         }
//     }    
//     if(ok) return ok;

//     account->loginStatus = Account::LOG_ERROR;
//     //search error msg
//     if(header.contentLength() == 0 && response.length() == 0) {
//         account->loginErrorMessage = tr("Unknown error1");
//         return false;
//     }
//     QString errdiv = account->sc->errorMessageBegin;
//     int empos = response.indexOf(errdiv, 0, Qt::CaseInsensitive);
//     if(empos == -1) {
//         account->loginErrorMessage = tr("Unknown error2");
//         return false;
//     }
//     QString errend = account->sc->errorMessageEnd;
//     int emend = response.indexOf(errend, empos, Qt::CaseInsensitive);
//     if(emend == -1) {
//         account->loginErrorMessage = tr("Unknown error3");
//         return false;
//     }
    
//     account->loginErrorMessage = response.mid(empos + errdiv.length(), emend - empos - errdiv.length());

//     //next: 
//     QString errnostr = account->loginErrorMessage;
//     Q_ASSERT(errnostr.length() == 1);
//     QString emRedirectBegin = QString("<div  id=prompt%1 style=\"display:none\" class=\"box-login\">").arg(errnostr);
//     QString emRedirectEnd = "</div>";
//     empos = response.indexOf(emRedirectBegin, 0, Qt::CaseInsensitive);
//     emend = response.indexOf(emRedirectEnd, empos, Qt::CaseInsensitive);
//     Q_ASSERT(empos != -1);
//     Q_ASSERT(emend != -1);

//     account->loginErrorMessage = response.mid(empos + emRedirectBegin.length(), emend - empos - emRedirectBegin.length());
//     account->loginErrorMessage = account->loginErrorMessage.trimmed();

//     account->loginErrorMessage = account->loginErrorMessage.replace("&nbsp;", "", Qt::CaseInsensitive);
//     empos = account->loginErrorMessage.indexOf(">");
//     emend = account->loginErrorMessage.indexOf("<", empos);
//     if(empos >= 0 && emend >= 0) {
//         account->loginErrorMessage = account->loginErrorMessage.mid(empos+1, emend - empos -1 );
//     }
    
//     return false;
// }
// bool SiteConst::parseKaixinFetchFeedResponse(Account *account)
// {
//     qDebug()<<"fetch feed result parser for kaixin";
//     char *input = NULL;
//     TidyDoc tdoc = NULL;
//     TidyNode anode = NULL;

//     input = this->htmlToUTF8(account);
//     if(input == NULL) {
//         return false;
//     }

//     tdoc = this->createAndCleanTidyDoc(input);
//     Q_ASSERT(tdoc != NULL);
    
//     anode = this->searchNode(tdoc, NULL, 
//                              account->sc->feedContentBegin.toAscii().data(), TidyTag_DIV);
//     if(anode == NULL) {
//         q_debug()<<"can not find feed node";
//         //TODO cleanup
//         return false;
//     }
//     //test
//     TidyBuffer abuff = {0};
//     tidyNodeGetText(tdoc, anode, &abuff);
//     //q_debug()<<account->sc->codec->toUnicode(QString((char*)(abuff.bp)).toAscii());

//     //parser body
//     QList<FeedRecord*> allFeeds;
//     QDateTime nowDate = QDateTime::currentDateTime();
//     QDateTime currDate = nowDate;
//     QString yesterday = "昨天";

//     TidyNode root = anode;
//     TidyNode ul;
//     TidyNode li;
//     ctmbstr name;
//     int iday = 0;

//     q_debug()<<"begin parser feed content node"<<root;
//     TidyNode img;
//     TidyNode hh;
//     TidyAttr attr;
//     for(ul = tidyGetChild(root); ul; ul = tidyGetNext(ul)) {
//         li = tidyGetChild(ul);
//         //get img symbol
//         img = tidyGetChild(li);
//         name = tidyNodeGetName(img);
//         qDebug()<<name;
//         if(tidyNodeGetId(img) != TidyTag_IMG) {
//             break;
//         }
//         Q_ASSERT(tidyNodeGetId(img) == TidyTag_IMG);
//         FeedRecord *fr = new FeedRecord();
//         fr->account = account;
        
//         for(attr = tidyAttrFirst(img); attr ; attr = tidyAttrNext(attr)) {
//             if(tidyAttrGetId(attr) == TidyAttr_SRC) {
//                 name = tidyAttrValue(attr);
//                 qDebug()<<"        "<<name;
//                 break;
//             }
//         }
//         fr->type = getXiaoneiFeedTypeImageName(QString(name));
//         fr->trends = QString(name);

//         TidyBuffer tbuf = {0};
//         TidyBuffer dbuf = {0};
//         tidyBufInit(&tbuf);
//         tidyBufInit(&dbuf);

//         //// get msg
//         li = tidyGetNext(li);
//         get_node_text3(tdoc, li, &tbuf, "vvvvvvvv", &dbuf);
//         QString msg = QString((char*)tbuf.bp);
//         msg = codec->toUnicode(msg.toAscii());
//         qDebug()<<"      "<<msg;

//         //get time
//         li = tidyGetNext(li);
//         tidyBufClear(&dbuf);
//         get_node_text(tdoc, li, &dbuf);
//         QString datetime = QString((char*)dbuf.bp).trimmed();
//         currDate = QDateTime::fromString(datetime, "yyyy-MM-dd hh:mm");
//         iday = currDate.daysTo(nowDate);
//         qDebug()<<"        "<<currDate;

//         tidyBufFree(&tbuf);
//         tidyBufFree(&dbuf);
        
//         fr->content = msg;
//         fr->date = currDate;
//         fr->day = iday;

//         allFeeds.append(fr);
//     }

//     qDebug()<<"have days: "<<iday;
//     account->tmpFeeds = allFeeds;

//     //cleanup
//     free(input);
//     this->cleanupTidyDoc(tdoc);
    
//     return true;
// }
// bool SiteConst::parseUCHomeLoginResponse(const QHttpResponseHeader &header, const QString &response, Account *account)
// {
//     q_debug()<<"login result parser for uchome";    

//     //for has checkcode uchome site
//     if(this->hasCheckCode) {
//         return this->parseUCHomeLoginResponseEx(header, response, account);
//     }

//     //for non checkcode uchome site
//     q_debug()<<"login result parser for non check code uchome";    
//     //dumpHttpHeader(header);    
//     //q_debug()<<account->sc->codec->toUnicode(response.toAscii());

//     bool ok = false;
//     account->cookies.clear();
//     QString encodedUserName = QUrl::toPercentEncoding(account->userName);
//     QList<QPair<QString, QString> > values = header.values();
//     for(int i = 0; i < values.count() ; i++) {
//         if(values.at(i).first == QString("Set-Cookie")) {
//             QNetworkCookie nc();
//             if(values.at(i).second.indexOf("deleted") == -1) {
//                 account->cookies.append(QPair<QString, QString>("Cookie", values.at(i).second));
//             }
//             if(values.at(i).second.left(strlen("uchome_loginuser=") + encodedUserName.length()) 
//                == QString("uchome_loginuser=%1").arg(encodedUserName)) {
//                 qDebug()<<"login success";
//                 account->loginStatus = Account::LOG_LOGGED;
//                 //return true;
//                 ok = true;
//             }
//         }
//     }    
//     if(ok) return ok;

//     account->loginStatus = Account::LOG_ERROR;
//     //search error msg
//     if(header.contentLength() == 0 && response.length() == 0) {
//         account->loginErrorMessage = tr("Unknown error1");
//         return false;
//     }
//     QString errdiv = account->sc->errorMessageBegin;
//     int empos = response.indexOf(errdiv);
//     if(empos == -1) {
//         account->loginErrorMessage = tr("Unknown error2");
//         return false;
//     }
//     QString errend = account->sc->errorMessageEnd;
//     int emend = response.indexOf(errend, empos);
//     if(emend == -1) {
//         account->loginErrorMessage = tr("Unknown error3");
//         return false;        
//     }
    
//     account->loginErrorMessage = response.mid(empos + errdiv.length(), emend - empos - errdiv.length());

//     //next: 
//     QString errnostr = account->loginErrorMessage;
//     Q_ASSERT(errnostr.length() == 1);
//     QString emRedirectBegin = QString("<div  id=prompt%1 style=\"display:none\" class=\"box-login\">").arg(errnostr);
//     QString emRedirectEnd = "</div>";
//     empos = response.indexOf(emRedirectBegin);
//     emend = response.indexOf(emRedirectEnd, empos);
//     Q_ASSERT(empos != -1);
//     Q_ASSERT(emend != -1);

//     account->loginErrorMessage = response.mid(empos + emRedirectBegin.length(), emend - empos - emRedirectBegin.length());
//     account->loginErrorMessage = account->loginErrorMessage.trimmed();
    
//     return false;
// }
// bool SiteConst::parseUCHomeLoginResponseEx(const QHttpResponseHeader &header, const QString &response, Account *account)
// {
//     qDebug()<<"login result parser for check code uchome";    
//     //dumpHttpHeader(header);
//     QString tmp = account->sc->codec->toUnicode(response.toAscii());
//     //q_debug()<<tmp;

//     /*
//       test cookie
// xs_seccode=fbfcSY%2Fbsr2ZJNJmcqYb3y1RHLPgkQ4kTXk%2Fxz96mK0u;
// xs_auth=30dbeHmoLy8hAzcXKJiInyDe73TM9kRLjGjK0yoqkIwYseCF6S6%2F9qE5Ly6klYQcR2TrVJwE6L%2BYHui9HHX15ZGuDUs;
// xs_loginuser=liuguangzhao;
// xs_synfriend=1

// xs_seccode=fbfcSY%2Fbsr2ZJNJmcqYb3y1RHLPgkQ4kTXk%2Fxz96mK0u;
// xs_auth=30dbeHmoLy8hAzcXKJiInyDe73TM9kRLjGjK0yoqkIwYseCF6S6%2F9qE5Ly6klYQcR2TrVJwE6L%2BYHui9HHX15ZGuDUs;
// xs_loginuser=liuguangzhao;
// xs_checkpm=1;
// xs_synfriend=1

// xs_seccode=fbfcSY%2Fbsr2ZJNJmcqYb3y1RHLPgkQ4kTXk%2Fxz96mK0u; xs_auth=30dbeHmoLy8hAzcXKJiInyDe73TM9kRLjGjK0yoqkIwYseCF6S6%2F9qE5Ly6klYQcR2TrVJwE6L%2BYHui9HHX15ZGuDUs; xs_loginuser=liuguangzhao; xs_checkpm=1; xs_synfriend=1
//      */
    
//     account->cookies.clear();

//     // account->cookies.append(QPair<QString,QString>("Cookie", "xs_seccode=fbfcSY%2Fbsr2ZJNJmcqYb3y1RHLPgkQ4kTXk%2Fxz96mK0u; path=/; domain=u.discuz.net; expires=Mon, 01-Sep-2008 23:48:39 GMT"));
//     // account->cookies.append(QPair<QString,QString>("Cookie", "xs_auth=30dbeHmoLy8hAzcXKJiInyDe73TM9kRLjGjK0yoqkIwYseCF6S6%2F9qE5Ly6klYQcR2TrVJwE6L%2BYHui9HHX15ZGuDUs; path=/; domain=u.discuz.net; expires=Mon, 01-Sep-2008 23:48:39 GMT"));
//     // account->cookies.append(QPair<QString,QString>("Cookie", "xs_loginuser=liuguangzhao; path=/; domain=u.discuz.net; expires=Mon, 01-Sep-2008 23:48:39 GMT"));
//     // account->cookies.append(QPair<QString,QString>("Cookie", "xs_checkpm=1; path=/; domain=u.discuz.net; expires=Mon, 01-Sep-2008 23:48:39 GMT"));
//     // account->cookies.append(QPair<QString,QString>("Cookie", "xs_synfriend=1; path=/; domain=u.discuz.net; expires=Mon, 01-Sep-2008 23:48:39 GMT"));

//     //account->cookies.append(QPair<QString, QString>("Cookie", "xs_seccode=9d6a2EeCaJficm70Dvrr%2FN6u3QeoYgbjASpFR2D0fCFs; xs_auth=35fal5WXD68HHE3nqlLP1G8rTX%2BJ2CwyuTHzGGwYt0Zq9QP1tGjcoD%2B1agA%2BUWZhe1WAnzlCRghj5AR3O29i%2BLGTsdU; xs_loginuser=liuguangzhao; xs_checkpm=1; xs_synfriend=1"));
//     //account->cookies.append(QPair<QString, QString>("Cookie", "xs_seccode=fbfcSY%2Fbsr2ZJNJmcqYb3y1RHLPgkQ4kTXk%2Fxz96mK0u; xs_auth=30dbeHmoLy8hAzcXKJiInyDe73TM9kRLjGjK0yoqkIwYseCF6S6%2F9qE5Ly6klYQcR2TrVJwE6L%2BYHui9HHX15ZGuDUs; xs_loginuser=liuguangzhao; xs_checkpm=1; xs_synfriend=1;"));

//     //QList<QPair<QString,QString> > cookies = CookieParser::getFireFoxCookiesByHost("u.discuz.net");
//     QList<QNetworkCookie> cookies ;
//     //qDebug()<<cookies;
//     QString cookieString;    
//     QString host = QUrl(account->sc->homePage).host();
    
// #ifdef Q_OS_WIN
//     cookies =  CookieParser::getIECookiesByHostEx(host);
//     if(cookies.count() == 0) {
//         q_debug()<<"no cookie for "<<host<<" found";
//         cookies = CookieParser::getFireFoxCookiesByHostEx(host);
//     }else{
//         q_debug()<<host<<"'s cookie found in IE";
//     }
// #else
//     cookies = CookieParser::getFireFoxCookiesByHostEx(host);
//     if(cookies.count() == 0) {
//         q_debug()<<"no cookie for "<<host<<" found";
//         cookies =  CookieParser::getIECookiesByHostEx(host);
//     }else{
//         q_debug()<<host<<"'s cookie found in FireFox";
//     }
// #endif

//     if(cookies.count() == 0) {
//         q_debug()<<"no cookie for "<<host<<" found";
//         account->loginErrorMessage = QString(tr("No cookies for %1 found.")).arg(host);
//         account->loginStatus = Account::LOG_ERROR;
//         return false;
//     }   
//     q_debug()<<"login success";
//     account->loginStatus = Account::LOG_LOGGED;

//     for(int i = 0; i < cookies.count(); i++) {
//         //account->cookies.append(QPair<QString, QString>("Cookie",cookies.at(i).second));
//         cookieString += QString("%1=%2")
//             .arg(QString(cookies.at(i).name()))
//             .arg(QString(cookies.at(i).value()));
//         if(i != cookies.count() - 1)  cookieString += "; ";
//     }
//     q_debug()<<cookieString;
    
//     account->cookies.append(QPair<QString, QString>("Cookie",cookieString));

//     return true;
// }

// bool SiteConst::parseUCHomeFetchFeedResponse(Account *account)
// {
//     qDebug()<<"fetch feed parser for uchome";    

//     char *input = NULL;
//     TidyDoc tdoc = NULL;
//     TidyNode anode = NULL;

//     input = this->htmlToUTF8(account);
//     if(input == NULL) {
//         return false;
//     }
//     //q_debug()<<account->sc->codec->toUnicode(account->loginOutput.toAscii());

//     tdoc = this->createAndCleanTidyDoc(input);
//     Q_ASSERT(tdoc != NULL);
    
//     anode = this->searchNode(tdoc, NULL, account->sc->feedContentBegin.toAscii().data(), TidyTag_DIV);
//     if(anode == NULL) {
//         q_debug()<<"can not find feed node";
//         //TODO cleanup
//         return false;
//     }

//     //parser body
//     QList<FeedRecord*> allFeeds;
//     QDateTime nowDate = QDateTime::currentDateTime();
//     QDateTime currDate = nowDate;
//     QString yesterday = "昨天";

//     TidyNode root = anode;
//     TidyNode ul;
//     TidyNode li;
//     ctmbstr name;
//     int iday = 0;

//     q_debug()<<"begin parser feed content node"<<root;
//     TidyNode img;
//     TidyNode hh;
//     TidyAttr attr;
//     for(ul = tidyGetChild(root); ul; ul = tidyGetNext(ul)) {
//         name = tidyNodeGetName(ul);
//         //qDebug()<<name;
//         if(tidyNodeGetId(ul) == TidyTag_H4) {
//             TidyBuffer tbuf = {0};
//             tidyBufInit(&tbuf);
//             //tidyNodeGetText(tdoc, ul, &tbuf);
//             get_node_text(tdoc, ul, &tbuf);
//             if(strstr((char*)tbuf.bp, "2008-") != NULL) {                
//                 QDate onlyDate = QDate::fromString(QString((char*)tbuf.bp).trimmed(), "yyyy-MM-dd");
//                 currDate.setDate(onlyDate);
//                 //q_debug()<<currDate<<onlyDate<<((char*)tbuf.bp);
//             }else{
//                 //must yesterday
//                 currDate = currDate.addDays(-1);
//             }
//             tidyBufFree(&tbuf);
//             continue;
//         }
     
        
//         if(tidyNodeGetId(ul) == TidyTag_UL) {
//             for(li = tidyGetChild(ul); li; li = tidyGetNext(li)) {

//                 TidyNode  tnode = NULL;
//                 TidyBuffer tbuf = {0};
//                 tidyBufInit(&tbuf);
//                 TidyBuffer tdate = {0};
//                 tidyBufInit(&tdate);
//                 tnode = searchNode(tdoc, li, NULL, TidyTag_IMG);
//                 Q_ASSERT(tnode != NULL);
//                 for(attr = tidyAttrFirst(tnode); attr ; attr = tidyAttrNext(attr)) {
//                     if(tidyAttrGetId(attr) == TidyAttr_SRC) {
//                         name = tidyAttrValue(attr);
//                         //qDebug()<<"        "<<name;
//                         break;
//                     }
//                 }
//                 QString imgPath = QString((char*)name);
            
//                 //new feed record
//                 FeedRecord *fr = new FeedRecord();
//                 fr->account = account;
//                 fr->type = getXiaoneiFeedTypeImageName(imgPath);
//                 if(fr->type.indexOf(".") == -1) {                    
//                     fr->type += ".gif";
//                 }else{                    
//                 }

//                 if(imgPath.startsWith("http")) {
//                     fr->trends = imgPath;
//                 }else{
//                     fr->trends = account->sc->homePage + "/" + imgPath;
//                 }
//                 //q_debug()<<fr->trends<<fr->type;

//                 get_node_text3(tdoc, li, &tbuf, "time", &tdate );
//                 QString fdate = QString((char*)tdate.bp);
            
//                 QString cc = QString((char*)tbuf.bp).trimmed();
//                 cc = rewriteRelativeLink(cc, account->sc->homeUri());
//                 cc = cc.right(cc.length() - (cc.indexOf("</a>")+4));
//                 cc = cc.left(cc.lastIndexOf("<a"));

//                 iday = currDate.daysTo(nowDate);
                
//                 fr->content = so->codec->toUnicode(cc.toAscii());
//                 //q_debug()<<fr->content<<"\n";
//                 fr->date = currDate;  
//                 fr->day = iday;

//                 allFeeds.append(fr);

//                 tidyBufFree(&tbuf);
//                 tidyBufFree(&tdate);

//             }
//         }
//     }

//     qDebug()<<"have days: "<<iday;
//     account->tmpFeeds = allFeeds;

//     //cleanup
//     free(input);
//     this->cleanupTidyDoc(tdoc);
    
//     return true;
// }
// bool SiteConst::parseUCHomeResolveSecureCodeResponse(Account *account)
// {
//     Q_ASSERT(account != NULL);

//     //<script>seccode();</script>
//     QString secCodeString = "<script>seccode();</script>";
//     if(account->loginOutput.indexOf(secCodeString) != -1) {
//         return true;
//     }

//     //default no checkcode
//     return false;
// }

// ///////////letter parser
// bool SiteConst::parseUnimpledFetchLetterResponse(Account *account, int letterType)
// {
//     q_debug()<<"unimpled";
//     return false;
// }

// bool SiteConst::parseHaineiFetchLetterResponse(Account *account, int letterType)
// {
//     q_debug()<<"parse  letter of hainei"<<letterType;
//     Q_ASSERT(account != NULL);
//     Q_ASSERT(letterType > SiteConst::LET_MIN && letterType < SiteConst::LET_MAX);

//     char *input = NULL;
//     TidyDoc tdoc = NULL;
//     TidyNode anode = NULL;
//     TidyNode tnode = NULL;
//     TidyBuffer tbuf = {0}; tidyBufInit(&tbuf);
//     QString  str;
//     const char * lclass = "pmlist";
//     QList<FeedRecord*> currLetters;
//     FeedRecord *fr = NULL;

//     input = this->htmlToUTF8(account, letterType);
//     if(input == NULL) {
//         return false;
//     }
//     //q_debug()<<account->sc->codec->toUnicode(account->loginOutput.toAscii());

//     tdoc = this->createAndCleanTidyDoc(input);
//     Q_ASSERT(tdoc != NULL);

//     anode = this->searchNode(tdoc, NULL, lclass, TidyTag_TABLE);
//     if(anode == NULL || (anode = tidyGetChild(anode)) == NULL) {
//         q_debug()<<"no letter found";
//         return false;
//     }
    
//     for(tnode = tidyGetChild(anode); tnode ; tnode = tidyGetNext(tnode)) {
//         tidyBufClear(&tbuf);
//         tidyNodeGetText(tdoc, tnode, &tbuf);
//         str = (char*)(tbuf.bp);
//         str = "<table>" + str + "</table>";
        
//         fr = new FeedRecord();
//         fr->account = account;
//         fr->content = account->sc->codec->toUnicode(str.toAscii());        
//         fr->content = this->rewriteRelativeLinkEx(fr->content, account->sc->rewriteLetterLink(account->sc->letterUrls.value(letterType).first));
//         //q_debug()<<fr->content;

//         currLetters.append(fr);
//     }        

//     free(input); input = NULL;
//     tidyBufFree(&tbuf); 

//     account->tmpLetters[letterType] = currLetters;

//     return true;
// }

// bool SiteConst::parseHaineiNoticeFetchLetterResponse(Account *account, int letterType)
// {
//     q_debug()<<"parse  notice letter of hainei"<<letterType;
//     Q_ASSERT(account != NULL);
//     Q_ASSERT(letterType > SiteConst::LET_MIN && letterType < SiteConst::LET_MAX);

//     char *input = NULL;
//     TidyDoc tdoc = NULL;
//     TidyNode anode = NULL;
//     TidyNode tnode = NULL;
//     TidyBuffer tbuf = {0}; tidyBufInit(&tbuf);
//     QString  str;
//     const char * lclass = "sect-content";
//     QList<FeedRecord*> currLetters;
//     FeedRecord *fr = NULL;

//     input = this->htmlToUTF8(account, letterType);
//     if(input == NULL) {
//         return false;
//     }
//     //q_debug()<<account->sc->codec->toUnicode(account->loginOutput.toAscii());

//     tdoc = this->createAndCleanTidyDoc(input);
//     Q_ASSERT(tdoc != NULL);

//     anode = this->searchNode(tdoc, NULL, lclass, TidyTag_DIV);
//     if(anode == NULL || (anode = tidyGetChild(anode)) == NULL) {
//         q_debug()<<"no letter found";
//         return false;
//     }
    
//     for(tnode = tidyGetChild(anode); tnode ; tnode = tidyGetNext(tnode)) {
//         tidyBufClear(&tbuf);
//         tidyNodeGetText(tdoc, tnode, &tbuf);
//         str = (char*)(tbuf.bp);
//         //str = "<table>" + str + "</table>";
        
//         fr = new FeedRecord();
//         fr->account = account;
//         fr->content = account->sc->codec->toUnicode(str.toAscii());        
//         fr->content = this->rewriteRelativeLinkEx(fr->content, account->sc->rewriteLetterLink(account->sc->letterUrls.value(letterType).first));
//         //q_debug()<<fr->content;

//         currLetters.append(fr);
//     }        

//     free(input); input = NULL;
//     tidyBufFree(&tbuf); 

//     account->tmpLetters[letterType] = currLetters;

//     return true;    
// }

// bool SiteConst::parseKaixinFetchLetterResponse(Account *account, int letterType)
// {
//     q_debug()<<"parse letter of kaixin"<<letterType;
//     Q_ASSERT(account != NULL);
//     Q_ASSERT(letterType > SiteConst::LET_MIN && letterType < SiteConst::LET_MAX);

//     char *input = NULL;
//     TidyDoc tdoc = NULL;
//     TidyNode anode = NULL;
//     TidyNode tnode = NULL;
//     const char * lclass = "dxx_of";

//     input = this->htmlToUTF8(account, letterType);
//     if(input == NULL) {
//         return false;
//     }
//     //q_debug()<<account->sc->codec->toUnicode(account->loginOutput.toAscii());

//     tdoc = this->createAndCleanTidyDoc(input);
//     Q_ASSERT(tdoc != NULL);

//     /////////base
//     anode = this->searchNode(tdoc, NULL, lclass, TidyTag_DIV);
//     if(anode == NULL) {
//         q_debug()<<"can not find private letter node";
//         //TODO cleanup, maybe no letter
//         return false;
//     }

//     QList<FeedRecord*> currLetters;
//     QString nodeText;
//     QString headerText;
//     TidyBuffer tbuf = {0};  tidyBufInit(&tbuf);    
//     TidyBuffer dbuf = {0};  tidyBufInit(&dbuf);    
//     ctmbstr strval = NULL;
//     TidyAttr attr = NULL;
//     FeedRecord *letter = NULL;

//     //header
//     // tnode = tidyGetHead(tdoc);
//     // tidyNodeGetText(tdoc, tnode, &tbuf);
//     // headerText = (char*)(tbuf.bp);
//     // tidyBufClear(&tbuf);        
//     // letter->header = headerText;
//     // q_debug()<<headerText;        
//     headerText = this->get_html_head_css(tdoc, &tbuf);
//     tidyBufClear(&tbuf);    
//     q_debug()<<headerText;

//     do {
//         tidyBufClear(&tbuf);
//         tidyNodeGetText(tdoc, anode, &tbuf);
//         nodeText = (char*)(tbuf.bp);
//         tidyBufClear(&tbuf);
//         nodeText = so->codec->toUnicode(nodeText.toAscii());
//         nodeText = this->rewriteRelativeLinkEx(nodeText, account->sc->rewriteLetterLink(account->sc->letterUrls.value(letterType).first));
//         //q_debug()<<nodeText;
           
//         //letter object
//         letter = new FeedRecord();
//         letter->account = account;
//         letter->content = nodeText;
//         currLetters.append(letter);
//         letter->header = headerText;

//         //
//         tidyBufClear(&tbuf);
//         tidyBufClear(&dbuf);
//         this->get_node_text2(tdoc, anode, &tbuf, &dbuf);
//         headerText = (char*)(tbuf.bp);
//         headerText = account->sc->codec->toUnicode(headerText.toAscii());
//         headerText = this->rewriteRelativeLinkEx(nodeText, account->sc->rewriteLetterLink(account->sc->letterUrls.value(letterType).first));
//         q_debug()<<headerText;
//         letter->content = headerText;

//         //has next
//         anode = this->searchNext(tdoc, anode);
//         if(anode == NULL) break;

//     }while(true);
//     tidyBufFree(&tbuf);

//     free(input); input = NULL;

//     account->tmpLetters[letterType] = currLetters;

//     return true;
// }
// bool SiteConst::parseKaixinFetchPubMsgLetterResponse(Account *account, int letterType)
// {
//     q_debug()<<"parse pubmsg letter of kaixin"<<letterType;
//     Q_ASSERT(account != NULL);
//     Q_ASSERT(letterType > SiteConst::LET_MIN && letterType < SiteConst::LET_MAX);

//     char *input = NULL;
//     TidyDoc tdoc = NULL;
//     TidyNode anode = NULL;
//     TidyNode tnode = NULL;
//     const char * lclass = "l srr_tt";

//     input = this->htmlToUTF8(account, letterType);
//     if(input == NULL) {
//         return false;
//     }
//     //q_debug()<<account->sc->codec->toUnicode(account->loginOutput.toAscii());

//     tdoc = this->createAndCleanTidyDoc(input);
//     Q_ASSERT(tdoc != NULL);

//     /////////base
//     anode = this->searchNode(tdoc, NULL, lclass, TidyTag_DIV);
//     if(anode == NULL) {
//         q_debug()<<"can not find private letter node";
//         //TODO cleanup, maybe no letter
//         return false;
//     }

//     QList<FeedRecord*> currLetters;
//     QString nodeText;
//     QString headerText;
//     TidyBuffer tbuf = {0};  tidyBufInit(&tbuf);    
//     TidyBuffer dbuf = {0};  tidyBufInit(&dbuf);    
//     ctmbstr strval = NULL;
//     TidyAttr attr = NULL;
//     FeedRecord *letter = NULL;

//     //header
//     // tnode = tidyGetHead(tdoc);
//     // tidyNodeGetText(tdoc, tnode, &tbuf);
//     // headerText = (char*)(tbuf.bp);
//     // tidyBufClear(&tbuf);        
//     // letter->header = headerText;
//     // q_debug()<<headerText;        
//     headerText = this->get_html_head_css(tdoc, &tbuf);
//     tidyBufClear(&tbuf);    
//     q_debug()<<headerText;

//     do {
//         tidyBufClear(&tbuf);
//         tidyNodeGetText(tdoc, anode, &tbuf);
//         nodeText = (char*)(tbuf.bp);
//         tidyBufClear(&tbuf);
//         nodeText = so->codec->toUnicode(nodeText.toAscii());
//         nodeText = this->rewriteRelativeLinkEx(nodeText, account->sc->rewriteLetterLink(account->sc->letterUrls.value(letterType).first));
//         //q_debug()<<nodeText;
           
//         //letter object
//         letter = new FeedRecord();
//         letter->account = account;
//         letter->content = nodeText;
//         currLetters.append(letter);
//         letter->header = headerText;

//         //
//         tidyBufClear(&tbuf);
//         tidyBufClear(&dbuf);
//         this->get_node_text2(tdoc, anode, &tbuf, &dbuf);
//         headerText = (char*)(tbuf.bp);
//         headerText = so->codec->toUnicode(headerText.toAscii());
//         headerText = this->rewriteRelativeLinkEx(nodeText, account->sc->rewriteLetterLink(account->sc->letterUrls.value(letterType).first));
//         q_debug()<<headerText;
//         letter->content = headerText;

//         //has next
//         anode = searchNext(tdoc, anode);
//         if(anode == NULL) break;        
//     }while(true);
//     tidyBufFree(&tbuf);

//     free(input); input = NULL;

//     account->tmpLetters[letterType] = currLetters;

//     return true;
// }

// bool SiteConst::parseXiaoneiFetchLetterResponse(Account *account, int letterType)
// {
//     q_debug()<<"parse  letter of xiaonei"<<letterType;
//     Q_ASSERT(account != NULL);
//     Q_ASSERT(letterType > SiteConst::LET_MIN && letterType < SiteConst::LET_MAX);

//     char *input = NULL;
//     TidyDoc tdoc = NULL;
//     TidyNode anode = NULL;
//     TidyNode tnode = NULL;
//     TidyBuffer tbuf = {0}; tidyBufInit(&tbuf);
//     QString  str;
//     const char * lclass = "message_rows";
//     QList<FeedRecord*> currLetters;
//     FeedRecord *fr = NULL;

//     input = this->htmlToUTF8(account, letterType);
//     if(input == NULL) {
//         return false;
//     }
//     //q_debug()<<account->sc->codec->toUnicode(account->loginOutput.toAscii());

//     tdoc = this->createAndCleanTidyDoc(input);
//     Q_ASSERT(tdoc != NULL);

//     anode = this->searchNode(tdoc, NULL, lclass, TidyTag_TABLE);
//     if(anode == NULL || (anode = tidyGetChild(anode)) == NULL) {
//         q_debug()<<"no letter found";
//         return false;
//     }
    
//     for(tnode = tidyGetChild(anode); tnode ; tnode = tidyGetNext(tnode)) {
//         tidyBufClear(&tbuf);
//         tidyNodeGetText(tdoc, tnode, &tbuf);
//         str = (char*)(tbuf.bp);
//         str = "<table>" + str + "</table>";
        
//         fr = new FeedRecord();
//         fr->account = account;
//         fr->content = so->codec->toUnicode(str.toAscii());        
//         fr->content = this->rewriteRelativeLinkEx(fr->content, account->sc->rewriteLetterLink(account->sc->letterUrls.value(letterType).first));
//         //q_debug()<<fr->content;

//         currLetters.append(fr);
//     }        

//     free(input); input = NULL;
//     tidyBufFree(&tbuf); 

//     account->tmpLetters[letterType] = currLetters;

//     return true;
// }


// bool SiteConst::parseUCHomeFetchLetterResponse(Account *account, int letterType)
// {
//     q_debug()<<"parse  letter of uchome"<<letterType;
//     Q_ASSERT(account != NULL);
//     Q_ASSERT(letterType > SiteConst::LET_MIN && letterType < SiteConst::LET_MAX);

//     char *input = NULL;
//     TidyDoc tdoc = NULL;
//     TidyNode anode = NULL;
//     TidyNode tnode = NULL;
//     TidyBuffer tbuf = {0}; tidyBufInit(&tbuf);
//     QString  str;
//     const char * lclass = "linetable";
//     QList<FeedRecord*> currLetters;
//     FeedRecord *fr = NULL;

//     input = this->htmlToUTF8(account, letterType);
//     if(input == NULL) {
//         return false;
//     }
//     //q_debug()<<account->sc->codec->toUnicode(account->loginOutput.toAscii());

//     tdoc = this->createAndCleanTidyDoc(input);
//     Q_ASSERT(tdoc != NULL);

//     anode = this->searchNode(tdoc, NULL, lclass, TidyTag_TABLE);
//     if(anode == NULL  // || (anode = tidyGetChild(anode)) == NULL
//        ) {
//         q_debug()<<"no letter found";
//         return false;
//     }
    
//     for(tnode = tidyGetChild(anode); tnode ; tnode = tidyGetNext(tnode)) {
//         tidyBufClear(&tbuf);
//         tidyNodeGetText(tdoc, tnode, &tbuf);
//         str = (char*)(tbuf.bp);
//         str = "<table>" + str + "</table>";
        
//         fr = new FeedRecord();
//         fr->account = account;
//         fr->content = so->codec->toUnicode(str.toAscii());        
//         fr->content = this->rewriteRelativeLinkEx(fr->content, account->sc->rewriteLetterLink(account->sc->letterUrls.value(letterType).first));
//         //q_debug()<<fr->content;

//         currLetters.append(fr);
//     }        

//     free(input); input = NULL;
//     tidyBufFree(&tbuf); 

//     account->tmpLetters[letterType] = currLetters;
    
//     return true;
// }

// bool SiteConst::parseUCHomeNoticeFetchLetterResponse(Account *account, int letterType)
// {
//     q_debug()<<"parse  letter of uchome notice"<<letterType;
//     Q_ASSERT(account != NULL);
//     Q_ASSERT(letterType > SiteConst::LET_MIN && letterType < SiteConst::LET_MAX);

//     char *input = NULL;
//     TidyDoc tdoc = NULL;
//     TidyNode anode = NULL;
//     TidyNode tnode = NULL;
//     TidyBuffer tbuf = {0}; tidyBufInit(&tbuf);
//     TidyBuffer dbuf = {0}; tidyBufInit(&dbuf);
//     QString  str;
//     const char * lclass = "feed";
//     QList<FeedRecord*> currLetters;
//     FeedRecord *fr = NULL;

//     input = this->htmlToUTF8(account, letterType);
//     if(input == NULL) {
//         return false;
//     }
//     //q_debug()<<account->sc->codec->toUnicode(account->loginOutput.toAscii());

//     tdoc = this->createAndCleanTidyDoc(input);
//     Q_ASSERT(tdoc != NULL);

//     anode = this->searchNode(tdoc, NULL, lclass, TidyTag_UL);
//     if(anode == NULL  // || (anode = tidyGetChild(anode)) == NULL
//        ) {
//         q_debug()<<"no letter found";
//         return false;
//     }
    
//     for(tnode = tidyGetChild(anode); tnode ; tnode = tidyGetNext(tnode)) {
//         tidyBufClear(&tbuf);
//         tidyBufClear(&dbuf);
//         //tidyNodeGetText(tdoc, tnode, &tbuf);
//         this->get_node_text3(tdoc, tnode, &tbuf, "",  &dbuf);
//         str = (char*)(tbuf.bp);
//         str = "<P>" + str + "</P>";
        
//         fr = new FeedRecord();
//         fr->account = account;
//         fr->content = so->codec->toUnicode(str.toAscii());        
//         fr->content = this->rewriteRelativeLinkEx(fr->content, account->sc->rewriteLetterLink(account->sc->letterUrls.value(letterType).first));
//         q_debug()<<fr->content;

//         currLetters.append(fr);
//     }        

//     free(input); input = NULL;
//     tidyBufFree(&tbuf); 
//     tidyBufFree(&dbuf); 

//     account->tmpLetters[letterType] = currLetters;    
//     return true;
// }

