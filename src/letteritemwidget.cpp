#include <QtWebKit>

#include "systemoption.h"
#include "useroption.h"
#include "threadfunctor.h"
#include "letteritemwidget.h"

const QString LetterItemWidget::feedTemplate = 
    "<html><body><table width=\"100%\"  border=\"0\">"
    "<tr>"
    "<td width=\"20\"></td><td align=\"right\">%1<img src=\"%2\" alt=\"%3\"></td>"
    "</tr>"
    "<tr>"
    "<td>%4</td><td>%5</td>"
    "</tr>"
    "<tr>"
    "<td></td><td align=\"right\">%6</td>"
    "</tr>"
    "</table></body></html>";

LetterItemWidget::LetterItemWidget(FeedRecord *feed, QWidget *parent)
    : QWidget(parent),
      sizeAdjusted(false)
{
    this->ui_win.setupUi(this);
    QObject::connect(this->ui_win.label, SIGNAL(linkActivated(const QString &)),
                     this, SLOT(activeLink(const QString &)));
    QObject::connect(this->ui_win.label, SIGNAL(linkHovered(const QString &)),
                     this, SLOT(hoverLink(const QString &)));
    //
	QString belongsTo = this->windowTitle().split(QChar('|')).at(1);
    this->feed = feed;
    Q_ASSERT(feed != NULL);
    Q_ASSERT(feed->account != NULL);
    //
    //blog of liu@tom on hainei
    QString title;
    if(feed->type.indexOf(".gif") == -1
       && feed->type.indexOf(".jpg") == -1) {
        title = QString(tr("%1 of %2 on "))
            .arg(feed->type)
            .arg(feed->account->userName);
    }else{
        //got image if not exists
        if(!QDir().exists(so->cachePath + "/" + feed->type)
           || QFileInfo(so->cachePath + "/" + feed->type).size() == 0) {
            //functorFetchIcon(feed->trends);
             QFuture<int> future = QtConcurrent::run(functorFetchIcon, feed->trends);
             //future.waitForFinished();
        }
        title = QString(tr("%1 on "))
            .arg(feed->account->userName);
    }

    QString feedType = "";
    QString feedMsg = feed->content;
    feedMsg = feedMsg.replace(QChar('\n'), QChar(' '));
    
    if(feed->type.indexOf(".gif") != -1
       || feed->type.indexOf(".jpg") != -1) {
        // feedMsg = QString("<img src=\"%1/%2\"/>")
        //     .arg(so->cachePath).arg(feed->type)  +  feedMsg;
        
        feedType = 
#ifdef Q_OS_WIN
			QString("<img src=\"%1/%2\"/>")
#else
            QString("<img src=\"file://%1/%2\"/>")
#endif
            .arg(so->cachePath).arg(feed->type);
    }
    this->ui_win.label->setText(this->feedTemplate.arg(title)
                                .arg(
#ifndef Q_OS_WIN
                                     QString("file://") +
#endif
                                     so->iconPath + "/" // + feed->account->sc->siteIcon
                                     ,
                                     // feed->account->sc->siteName
                                     "",
                                     feedType,
                                     feedMsg,
                                     feed->date.toString()));

    this->ui_win.label->setVisible(false); // hide the content
    
    this->ui_win.webView->setHtml(this->ui_win.label->text());
    //QObject::connect(this->ui_win.webView->page()->mainFrame(), SIGNAL(initialLayoutCompleted()),
    //               this, SLOT(autoAdjustWebViewSize()));

    this->wv = this->ui_win.webView;
    this->wp = this->wv->page();
    this->frm = this->wp->mainFrame();
    //this->wv->setContextMenuPolicy(Qt::CustomContextMenu);
    this->wp->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);//info us all user clicked link
    this->wp->setForwardUnsupportedContent(true);
    this->frm->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    
    QObject::connect(this->wv, SIGNAL(customContextMenuRequested(const QPoint &)),
                     this, SLOT(wkContextMenuRequested(const QPoint &)));
    QObject::connect(this->wp, SIGNAL(linkClicked(const QUrl &)),
                     this, SLOT(wkActiveLink(const QUrl &)));
    QObject::connect(this->wp, SIGNAL(linkHovered(const QString&, const QString &, const QString &)),
                     this, SLOT(wkHoverLink(const QString&, const QString &, const QString &)));
    QObject::connect(this->wv, SIGNAL(urlChanged(const QUrl &)),
                     this, SLOT(wkChangeUrl(const QUrl &)));
    QObject::connect(this->wp, SIGNAL(geometryChangeRequested(const QRect&)),
                     this, SLOT(wkChangeGeometry(const QRect &)));
    QObject::connect(this->wp, SIGNAL(repaintRequested(const QRect&)),
                     this, SLOT(wkRequestRepaint(const QRect&)));
    QObject::connect(this->wp, SIGNAL(scrollRequested(int, int, const QRect&)),
                     this, SLOT(wkRequestScroll(int, int, const QRect&)));
    QObject::connect(this->wp, SIGNAL(statusBarMessage(const QString&)),
                     this, SLOT(wkStatusBarMessage(const QString&)));
    QObject::connect(this->wp, SIGNAL(loadFinished(bool)),
                     this, SLOT(wkLoadFinished(bool)));
    QObject::connect(this->wp, SIGNAL(downloadRequested(const QNetworkRequest &)),
                     this, SLOT(wkRequestDownload(const QNetworkRequest &)));
    QObject::connect(this->wp, SIGNAL(unsupportedContent(QNetworkReply *)),
                     this, SLOT(wkDealUnsupportedContent(QNetworkReply*)));
}

LetterItemWidget::~LetterItemWidget()
{

}

bool LetterItemWidget::equalsTo(FeedRecord *f)
{
    if(this->feed == f) return true;
    return false;
}
bool LetterItemWidget::similarToKeyword(QString keyword)
{
    if(this->feed->content.indexOf(keyword) != -1) {
        return true;
    }
    return false;
}

void LetterItemWidget::activeLink(const QString &link)
{
    QString decoded_link = QUrl::fromPercentEncoding(link.toAscii());
    q_debug()<<"vv"<<link<<decoded_link;
    
    QDesktopServices::openUrl(decoded_link);
}
void LetterItemWidget::hoverLink(const QString &link)
{
    //q_debug()<<"vv"<<link;
    if(!link.isEmpty()) {
        if(link.startsWith("http"))
            this->setToolTip(link);
        else{
            //this->setToolTip(this->feed->account->sc->homeUri() + link);
        }
    }else{
        this->setToolTip(QString::null);
    }
}
//no use , use showEvent is ok
void LetterItemWidget::show()
{
    QWidget::show();
}
void LetterItemWidget::showEvent ( QShowEvent * event )
{
    //q_debug()<<"?vvvvvvvvvvvvvvvvvvvvvvvvv";    
    QWidget::showEvent(event);
    //q_debug()<<this->ui_win.webView->page()->mainFrame()->renderTreeDump();
}

//自适应窗口高度              
void LetterItemWidget::autoAdjustWebViewSize()
{
    if(this->sizeAdjusted) return;

    //QSize size = this->ui_win.webView->page()->viewportSize();
    //q_debug()<<size;
    //QSize(352, 167) , QSize(516, 167)
    /*
"layer at (0,0) size 352x167
  RenderView at (0,0) size 352x167
layer at (0,0) size 352x34
  RenderBlock {HTML} at (0,0) size 352x34
    RenderBody {BODY} at (8,8) size 336x18
      RenderText {#text} at (0,0) size 38x17
        text run at (0,0) width 38: "dsfsdf"
      RenderText {#text} at (0,0) size 0x0
      RenderText {#text} at (0,0) size 0x0
"
    */
    QWebView *wv = this->ui_win.webView;
    QWebPage *wp = wv->page();
    //QWebPage *wp = new QWebPage();
    
    QWebFrame *wf = wp->mainFrame();
    QString tree;
    tree = wf->renderTreeDump();
    //q_debug()<<tree;
    int nl = tree.indexOf(QChar('\n'));
    tree = tree.left(nl);
    //q_debug()<<nl<<tree;
    QStringList tmp = tree.split(QChar('x'));
    //q_debug()<<tmp;
    wv->setFixedHeight(tmp.at(1).toInt());    

    this->sizeAdjusted = true;
}

void LetterItemWidget::wkActiveLink(const QUrl &url)
{
    //QString decoded_link = QUrl::fromPercentEncoding(link.toAscii());
    //q_debug()<<"vv"<<link<<decoded_link;
    
    //QDesktopServices::openUrl(decoded_link);
    q_debug()<<url;
    QDesktopServices::openUrl(url);
}
void LetterItemWidget::wkHoverLink(const QString &link, const QString &title, const QString &content)
{
    if(!link.isEmpty()) {
        if(link.startsWith("http"))
            this->setToolTip(link);
        else{
            //this->setToolTip(this->feed->account->sc->homeUri() + link);
        }
    }else{
        this->setToolTip(QString::null);
    }

    //q_debug()<<link<<title<<content;
}

void LetterItemWidget::wkContextMenuRequested(const QPoint &pos)
{
    q_debug()<<pos;
}

void LetterItemWidget::wkChangeUrl(const QUrl &url)
{
    q_debug()<<url;
}

void LetterItemWidget::wkChangeGeometry(const QRect &geom)
{
    q_debug()<<geom;
}

void LetterItemWidget::wkRequestRepaint(const QRect &dirtyRect)
{
    q_debug()<<dirtyRect;
}

void LetterItemWidget::wkRequestScroll ( int dx, int dy, const QRect & rectToScroll )
{
    q_debug()<<dx<<dy<<rectToScroll;
}
void LetterItemWidget::wkStatusBarMessage(const QString &text)
{
    //q_debug()<<text;
}
void LetterItemWidget::wkLoadFinished(bool ok)
{
    //q_debug()<<ok<<sender();
    this->autoAdjustWebViewSize();
}

void LetterItemWidget::wkRequestDownload(const QNetworkRequest & request)
{
    //q_debug()<<request;
    q_debug()<<"what?"<<request.url();
}

void LetterItemWidget::wkDealUnsupportedContent(QNetworkReply *reply)
{
    q_debug()<<reply;
}

