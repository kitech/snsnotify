#include <QtWebKit>

#include "systemoption.h"
#include "useroption.h"
#include "threadfunctor.h"
#include "feeditemwidget.h"

FeedItemWidget::FeedItemWidget(FeedRecord *feed, QWidget *parent)
    : QWidget(parent),
      sizeAdjusted(false)
{
    this->ui_win.setupUi(this);

	QString belongsTo = this->windowTitle().split(QChar('|')).at(1);
    this->feed = feed;
    Q_ASSERT(feed != NULL);
    Q_ASSERT(feed->account != NULL);
    //

    this->initFeedTemplate();

    this->wp = this->ui_win.webView->page();
    //this->wv->setContextMenuPolicy(Qt::CustomContextMenu);
    this->wp->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);//info us all user clicked link
    this->wp->setForwardUnsupportedContent(true);
    this->wp->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    this->wp->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    
    QObject::connect(this->ui_win.webView, SIGNAL(customContextMenuRequested(const QPoint &)),
                     this, SLOT(wkContextMenuRequested(const QPoint &)));
    QObject::connect(this->ui_win.webView, SIGNAL(urlChanged(const QUrl &)),
                     this, SLOT(wkChangeUrl(const QUrl &)));

    QObject::connect(this->wp, SIGNAL(linkClicked(const QUrl &)),
                     this, SLOT(wkActiveLink(const QUrl &)));
    QObject::connect(this->wp, SIGNAL(linkHovered(const QString&, const QString &, const QString &)),
                     this, SLOT(wkHoverLink(const QString&, const QString &, const QString &)));
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

FeedItemWidget::~FeedItemWidget()
{

}

bool FeedItemWidget::initFeedTemplate()
{
    //blog of liu@tom on hainei
   
    this->ui_win.webView->setHtml(this->feed->content);

    return true;
}

bool FeedItemWidget::equalsTo(FeedRecord *f)
{
    if(this->feed->equalsTo(f)) return true;
    return false;
}
bool FeedItemWidget::similarToKeyword(QString keyword)
{
    if(this->feed->content.indexOf(keyword) != -1) {
        return true;
    }
    return false;
}

void FeedItemWidget::activeLink(const QString &link)
{
    QString decoded_link = QUrl::fromPercentEncoding(link.toAscii());
    q_debug()<<"vv"<<link<<decoded_link;
    
    QDesktopServices::openUrl(decoded_link);
}
void FeedItemWidget::hoverLink(const QString &link)
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
void FeedItemWidget::show()
{
    QWidget::show();
}
void FeedItemWidget::showEvent ( QShowEvent * event )
{
    //q_debug()<<"?vvvvvvvvvvvvvvvvvvvvvvvvv";    
    QWidget::showEvent(event);
    //q_debug()<<this->ui_win.webView->page()->mainFrame()->renderTreeDump();
    this->autoAdjustWebViewSize();
}

//自适应窗口高度              
void FeedItemWidget::autoAdjustWebViewSize()
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

    //this->sizeAdjusted = true;
}

void FeedItemWidget::wkActiveLink(const QUrl &url)
{
    //QString decoded_link = QUrl::fromPercentEncoding(link.toAscii());
    //q_debug()<<"vv"<<link<<decoded_link;
    
    //QDesktopServices::openUrl(decoded_link);
    q_debug()<<url;
    QDesktopServices::openUrl(url);
}
void FeedItemWidget::wkHoverLink(const QString &link, const QString &title, const QString &content)
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

void FeedItemWidget::wkContextMenuRequested(const QPoint &pos)
{
    q_debug()<<pos;
}

void FeedItemWidget::wkChangeUrl(const QUrl &url)
{
    q_debug()<<url;
}

void FeedItemWidget::wkChangeGeometry(const QRect &geom)
{
    q_debug()<<geom;
}

void FeedItemWidget::wkRequestRepaint(const QRect &dirtyRect)
{
    //q_debug()<<dirtyRect;
}

void FeedItemWidget::wkRequestScroll ( int dx, int dy, const QRect & rectToScroll )
{
    q_debug()<<dx<<dy<<rectToScroll;
}
void FeedItemWidget::wkStatusBarMessage(const QString &text)
{
    //q_debug()<<text;
}
void FeedItemWidget::wkLoadFinished(bool ok)
{
    //q_debug()<<ok<<sender();
    this->autoAdjustWebViewSize();
}

void FeedItemWidget::wkRequestDownload(const QNetworkRequest & request)
{
    //q_debug()<<request;
    q_debug()<<"what?"<<request.url();
}

void FeedItemWidget::wkDealUnsupportedContent(QNetworkReply *reply)
{
    q_debug()<<reply;
}

