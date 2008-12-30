#include <QtNetwork>

#include "md5.h"
#include "utils.h"
#include "feed_record.h"


////////////
// QString FeedRecord::md5StringSum()
// {
//     static const char *hexchars = "0123456789ABCDEF";
//     unsigned char ch, chh4, chl4;
//     QString md5str;

//     for(int i = 0 ; i < 16; i ++) {
//         ch = this->md5sum[i];
//         chh4 = ch >> 4;
//         chl4 = ch << 4 >> 4;
//         md5str += QChar(hexchars[chh4]);
//         md5str += QChar(hexchars[chl4]);
//     }
    
//     Q_ASSERT(md5str.length() == 32);
//     return md5str;
// }

///////////////
SiteConst::SiteConst()
{
    this->u8codec = QTextCodec::codecForName("UTF-8");
}
SiteConst::~SiteConst()
{
}

QString SiteConst::combine_cookies(QStringList cookies)
{
    QString cookie_string;

    //Set-Cookie: uchome_loginuser=liuguangzhao; expires=Tue, 01-Dec-2009 12:47:06 GMT; path=/
    for(int i = cookies.count() - 1; i >= 0; i --) {
        cookie_string += cookies.at(i).split(":").at(1).split(";").at(0).trimmed();
        cookie_string += "; ";
    }
    this->cookies = cookie_string;
    return cookie_string;
}

QString SiteConst::combine_cookies2(QStringList cookies)
{
    QString cookie_string;

    //uchome_loginuser=liuguangzhao; expires=Tue, 01-Dec-2009 12:47:06 GMT; path=/
    for(int i = cookies.count() - 1; i >= 0; i --) {
        cookie_string += cookies.at(i).split(";").at(0).trimmed();
        cookie_string += "; ";
    }
    this->cookies = cookie_string;
    return cookie_string;
}

//html 必须是utf8编码的
TidyDoc  SiteConst::create_document_tree(QByteArray html)
{
    TidyDoc tdoc = NULL;
    TidyBuffer *errorBuffer;
    Bool ok;
    int rc = -1;
    const char *input = html.data();
    QString charset ;

    Q_ASSERT(input != NULL);
    Q_ASSERT(strlen(input) > 0);

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

    //不用下面这段代码效果更好，不会出来编码混乱的情况?
    // if(rc >= 0) {
    //     rc = tidyCleanAndRepair(tdoc);
    // }else{
    //     q_debug()<<"parse error";
    //     return tdoc;
    // }
    // if(rc >= 0) {
    //     rc = tidyRunDiagnostics(tdoc);
    // }else{
    //     q_debug()<<"clean error";
    // }

    q_debug()<<"HTML charset:"<<charset
             <<"tidyErrorCount :"<<tidyErrorCount(tdoc)
             <<"tidyWarningCount :"<<tidyWarningCount(tdoc);

    if(tidyErrorCount(tdoc) > 0) {
        this->error_message = "Invalid HTML code";
        q_debug()<<this->error_message;
        //q_debug()<<"error lines:"<<QString((char*)errorBuffer->bp);
        this->free_feed_document_tree(tdoc);
        return NULL;
    }

    //tidySaveStdout(tdoc);

    return tdoc;
}

int SiteConst::free_document_tree(TidyDoc doc)
{
    Q_ASSERT(doc != NULL);
    TidyBuffer *errbuf = NULL;

    errbuf = (TidyBuffer*)tidyGetAppData(doc);
    Q_ASSERT(errbuf != NULL);

    tidyBufFree(errbuf);
    tidyRelease(doc);

    return 0;    
}

QString SiteConst::md5(const char *s)
{
    unsigned char mem[20] = {0};
    const char *hexchars = "0123456789abcdef";
    unsigned char ch;
    unsigned char chh4, chl4;
    QString md5str;
    char c;
    
    Q_ASSERT(s != NULL);
    md5CheckSum(s, strlen(s), mem);

    for(int i = 0 ; i < 16; i ++) {
        ch = mem[i];
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

TidyDoc  SiteConst::create_feed_document_tree()
{
    TidyDoc tdoc = NULL;
    TidyBuffer *errorBuffer;
    Bool ok;
    int rc = -1;
    const char *input = this->feed_page_utf8_html.data();
    QString charset ;

    Q_ASSERT(input != NULL);
    Q_ASSERT(strlen(input) > 0);

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

    //不用下面这段代码效果更好，不会出来编码混乱的情况?
    // if(rc >= 0) {
    //     rc = tidyCleanAndRepair(tdoc);
    // }else{
    //     q_debug()<<"parse error";
    //     return tdoc;
    // }
    // if(rc >= 0) {
    //     rc = tidyRunDiagnostics(tdoc);
    // }else{
    //     q_debug()<<"clean error";
    // }

    q_debug()<<"HTML charset:"<<charset
             <<"tidyErrorCount :"<<tidyErrorCount(tdoc)
             <<"tidyWarningCount :"<<tidyWarningCount(tdoc);

    if(tidyErrorCount(tdoc) > 0) {
        this->error_message = "Invalid HTML code";
        q_debug()<<this->error_message;
        //q_debug()<<QString((char*)errorBuffer->bp);
        this->free_feed_document_tree(tdoc);
        return NULL;
    }

    //tidySaveStdout(tdoc);

    return tdoc;
}

int SiteConst::free_feed_document_tree(TidyDoc doc)
{
    Q_ASSERT(doc != NULL);
    TidyBuffer *errbuf = NULL;

    errbuf = (TidyBuffer*)tidyGetAppData(doc);
    Q_ASSERT(errbuf != NULL);

    tidyBufFree(errbuf);
    tidyRelease(doc);

    return 0;
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
            //qDebug()<<tidyNodeGetName(childNode);
            for(attr = tidyAttrFirst(childNode); attr; attr = tidyAttrNext(attr)) {
                if(tidyAttrGetId(attr) == TidyAttr_CLASS
                   ||
                   tidyAttrGetId(attr) == TidyAttr_ID
                   || 
                   tidyAttrGetId(attr) == TidyAttr_NAME ) {
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

void SiteConst::get_node_text(TidyDoc tdoc, TidyNode node, TidyBuffer *buf)
{
    TidyBuffer text = {0};    
    tidyBufInit(&text);

    switch(tidyNodeGetType(node)) {
    case TidyNode_Text:
        //q_debug()<<"text found";
        tidyNodeGetText(tdoc, node, &text);
        if(text.bp[text.size-1] == '\n') {
            text.size -= 1;
        }
        tidyBufAppend(buf, text.bp, text.size);
        break;
    default:
        switch(tidyNodeGetId(node)) {
        case TidyTag_A:
            //q_debug()<<"a node found";
            get_node_text(tdoc, tidyGetChild(node), buf);        
            break;
        case TidyTag_SPAN:
            get_node_text(tdoc, tidyGetChild(node), buf);
            break;
        case TidyTag_Q:
            get_node_text(tdoc, tidyGetChild(node), buf);
            break;
        case TidyTag_PLAINTEXT:
            //q_debug()<<"plain text found";
            tidyNodeGetText(tdoc, node, &text);
            tidyBufAppend(buf, text.bp, text.size);
            break;
        case TidyTag_P:
        case TidyTag_H4:
        case TidyTag_H6:
        case TidyTag_LI:
        case TidyTag_DIV:            
            TidyNode child;
            for(child = tidyGetChild(node) ; child ; child = tidyGetNext(child)) {
                get_node_text(tdoc, child, buf);
            }
            break;
        case TidyTag_IMG:
            //q_debug()<<"it is img";
            break;
        default:
            q_debug()<<"what is it?"<<tidyNodeGetName(node);
            break;
        }
        break;
    };
    tidyBufFree(&text);
}

void SiteConst::get_node_text3(TidyDoc tdoc, TidyNode node, TidyBuffer *buf, const char *date_class, TidyBuffer *dbuf)
{
    TidyBuffer text = {0};    
    TidyNode child;
    TidyAttr  attr;
    ctmbstr ptr = NULL;

    tidyBufInit(&text);    
    switch(tidyNodeGetType(node)) {
    case TidyNode_Text:
        //q_debug()<<"text found";
        tidyNodeGetText(tdoc, node, &text);
        if(text.bp[text.size-1] == '\n') {
            text.size -= 1;
        }
        tidyBufAppend(buf, text.bp, text.size);
        break;
    default:
        switch(tidyNodeGetId(node)) {
        case TidyTag_A:
            //if javascript , omit link, only text
            //attr = tidyAttrGetById(node, TidyAttr_HREF);
            attr = tidyAttrGetHREF(node);
            if(attr != NULL) {
                ptr = tidyAttrValue(attr);
                if(ptr != NULL) {
                    if(strlen(ptr) >= 10 && qstrnicmp(ptr, "javascript", 10) == 0) {
                        get_node_text3(tdoc, tidyGetChild(node), buf, date_class, dbuf);
                        break;
                    }
                    if(ptr[0] == '#') { //  anchor  tag
                        get_node_text3(tdoc, tidyGetChild(node), buf, date_class, dbuf);
                        break;
                    }
                }
            }
            attr = NULL; ptr = NULL;
            tidyBufClear(&text);
            tidyNodeGetText(tdoc, node, &text);
            tidyBufAppend(buf, text.bp, text.size);            
            tidyBufClear(&text);
            break;
        case TidyTag_SPAN:
            //q_debug()<<"a node found";
            attr = tidyAttrFirst(node);
            if(attr && tidyAttrGetId(attr) == TidyAttr_CLASS) {
                if(strcasecmp(tidyAttrValue(attr), date_class) == 0) {
                    //q_debug()<<" got a date";
                    if(dbuf->size != 0 ) {
                        //Q_ASSERT(dbuf->size == 0);
                        tidyBufClear(dbuf);
                    }
                    tidyNodeGetText(tdoc, tidyGetChild(node), dbuf);
                    break;
                }
            }
            goto get_all_child_text;
            break;
        case TidyTag_PLAINTEXT:
            //q_debug()<<"plain text found";
            tidyNodeGetText(tdoc, node, &text);
            tidyBufAppend(buf, text.bp, text.size);
            break;
        case TidyTag_P:
            if(buf->size > 0) {
                tidyBufAppend(buf, (void*)"<BR>", 4);
            }
        case TidyTag_H2:
        case TidyTag_H3:
        case TidyTag_H6:
        //case TidyTag_A:
        case TidyTag_Q:
        case TidyTag_UL:
        case TidyTag_LI:
        case TidyTag_DIV:
        get_all_child_text:
            for(child = tidyGetChild(node) ; child ; child = tidyGetNext(child)) {
                get_node_text3(tdoc, child, buf, date_class, dbuf);
            }
            break;
        default:
            break;
        }
        break;
    };
    tidyBufFree(&text);
    
}
QString SiteConst::format_human_feed(FeedRecord *rec)
{
    return rec->content;
}
QString SiteConst::format_human_note(FeedRecord *rec)
{
    
    return rec->content;
    return QString::null;
}
QString SiteConst::format_human_friend(FeedRecord *rec)
{
    return rec->content;
}
int SiteConst::get_url(QString url, QByteArray &html, QString refer)
{
    Q_ASSERT(url.length() > 0);

    //QByteArray html;
    QUrl u(url);
    Q_ASSERT(u.isValid());

    QTcpSocket *sock = NULL;
    sock = new QTcpSocket();    
    sock->connectToHost(u.host(), u.port(80));
    if(!sock->waitForConnected()) {
        q_debug()<<"Connect error:"<<u.host()<<u.port(80);        
        return FETCH_FEED_ERROR;     
    }
    
    QString request;
    request = 
        QString(
                "GET %1 HTTP/1.1\r\n"
                "Host: %2\r\n"
                "User-Agent: Opera/9.62 (X11; Linux i686; U; zh-cn)\r\n"
                "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
                "Accept-Language: zh-cn,zh;q=0.5\r\n"
                "Accept-Charset: gb2312,utf-8;q=0.7,*;q=0.7\r\n"
                //"Keep-Alive: 300\r\n"
                //"Connection: keep-alive\r\n"
                "Connection: close\r\n"
                "Cookie: %3\r\n"
                "Referer: %4\r\n"
                "\r\n"
                )
        .arg(u.path() + "?" + u.encodedQuery(), u.host(), this->cookies,
             refer);

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

    //QByteArray html;
    html.clear();
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
    
    //this->feed_page_html = html;
    q_debug()<<"read len =? content-length: "<<html.length()<<" =? "<<content_length;
    q_debug()<<html.left(6)<<html.right(20);

    //this->clean_my_name_lable();

    return FETCH_FEED_OK;    
    //return html;
}

