#ifndef LETTER_ITEM_WIDGET
#define LETTER_ITEM_WIDGET


#include <QtCore>
#include <QtGui>
#include <QtWebKit>

#include "utils.h"

#include "ui_feeditemwidget.h"

class FeedRecord;

class LetterItemWidget : public QWidget
{
    Q_OBJECT;
public:
    LetterItemWidget(FeedRecord *feed, QWidget *parent = 0);
    ~LetterItemWidget();

    bool equalsTo(FeedRecord *f);
    bool similarToKeyword(QString keyword);
    
public slots:
    void show();

private slots:
    void activeLink(const QString &link);
    void hoverLink(const QString &link);
    void autoAdjustWebViewSize();
    void wkActiveLink(const QUrl &url);
    void wkHoverLink(const QString &link, const QString &title, const QString &content);
    void wkContextMenuRequested(const QPoint &pos);
    void wkChangeUrl(const QUrl &url);
    void wkChangeGeometry(const QRect &geom);
    void wkRequestRepaint(const QRect &dirtyRect);
    void wkRequestScroll ( int dx, int dy, const QRect & rectToScroll );
    void wkStatusBarMessage(const QString &text);
    void wkLoadFinished ( bool ok );
    void wkRequestDownload(const QNetworkRequest & request);
    void wkDealUnsupportedContent(QNetworkReply *reply);

protected:
    void showEvent ( QShowEvent * event );

private:
    FeedRecord *feed;
    Ui::FeedItemWidget ui_win;
    bool sizeAdjusted;
    QWebView *wv;
    QWebPage *wp;
    QWebFrame *frm;
    static const QString feedTemplate;    
};

#endif
